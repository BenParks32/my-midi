
#include <Arduino.h>
#include "Mode.h"

const byte MAX_PATCH = 39;

const byte BUTTON_ONE = 1;
const byte BUTTON_TWO = 2;
const byte BUTTON_THREE = 3;
const byte BUTTON_FOUR = 4;

const byte LIGHT_ONE = 0;
const byte LIGHT_TWO = 1;
const byte LIGHT_THREE = 2;
const byte LIGHT_FOUR = 3;

const byte SCREEN_ONE = 0;
const byte SCREEN_TWO = 1;
const byte SCREEN_THREE = 2;
const byte SCREEN_FOUR = 3;

const byte STOMP_SNAPSHOT_CC = 69;
const byte STOMP_TUNER_CC = 68;
const byte STOMP_LOOP_RECORD_OVERDUB_CC = 60;
const byte STOMP_LOOP_PLAY_STOP_CC = 61;
const byte STOMP_LOOP_PLAY_ONCE_CC = 62;
const byte STOMP_LOOP_UNDO_REDO_CC = 63;
const byte STOMP_LOOP_FORWARD_REV_CC = 65;
const byte STOMP_LOOP_FULL_HALF_SPEED_CC = 66;

const byte STOMP_LOOP_OVERDUB_VAL = 0;
const byte STOMP_LOOP_RECORD_VAL = 64;
const byte STOMP_LOOP_STOP_VAL = 0;
const byte STOMP_LOOP_PLAY_VAL = 64;
const byte STOMP_LOOP_PLAY_ONCE_VAL = 64;
const byte STOMP_LOOP_UNDO_REDO_VAL = 64;
const byte STOMP_LOOP_FORWARD_VAL = 0;
const byte STOMP_LOOP_REVERSE_VAL = 64;
const byte STOMP_LOOP_FULL_SPEED_VAL = 0;
const byte STOMP_LOOP_HALF_SPEED_VAL = 0;

const byte STOMP_MIDI_CHANNEL = 1;
const byte AMP_MIDI_CHANNEL = 2;

const byte NORMAL_MODE = 0;
const byte BANK_MODE = 1;
const byte LOOPER_MODE = 2;

//////////////////////////////////
// Bank Mode
BankMode::BankMode(midi_t& midi, const LightManager& lightManager, Screen** ppScreens) :
  _midi(midi),
  _lightManager(lightManager),
  _ppScreens(ppScreens),
  _activeBank(0)
{
}

void BankMode::activate()
{
  SymbolGlyph arrowDown(ARROW_DOWN);
  SymbolGlyph arrowUp(ARROW_UP);
  BankNumberGlyph bankNumber(_activeBank);

  const char* modeLines[2] = { "Select", "Bank" };
  ModeGlyph mode(modeLines, 2);

  _ppScreens[SCREEN_ONE]->draw(arrowDown);
  _ppScreens[SCREEN_TWO]->draw(bankNumber);
  _ppScreens[SCREEN_THREE]->draw(arrowUp);  
  _ppScreens[SCREEN_FOUR]->draw(mode);  

  _lightManager.turnAllOff();
  _lightManager.turnOn(LIGHT_ONE);  
  _lightManager.turnOn(LIGHT_THREE);
  _lightManager.setFlashing(LIGHT_ONE, false);
  _lightManager.setFlashing(LIGHT_TWO, false);
  _lightManager.setFlashing(LIGHT_THREE, false);
  _lightManager.setFlashing(LIGHT_FOUR, true);
}

void BankMode::buttonPressed(const byte number)
{
  switch(number)
  {
    case BUTTON_THREE:
    {
      if(_activeBank > 0)
        --_activeBank;
      else
        _activeBank = MAX_PATCH;
    } break;
    
    case BUTTON_ONE:
    {
      if(_activeBank < MAX_PATCH)
        ++_activeBank;
      else
        _activeBank = 0;
    } break;

    default:
      break;
  }

  BankNumberGlyph bankNumber(_activeBank);
  _ppScreens[SCREEN_TWO]->draw(bankNumber);
  _midi.sendProgramChange(_activeBank, STOMP_MIDI_CHANNEL);
}

void BankMode::buttonLongPressed(const byte number)
{
}

byte BankMode::getBank() const
{
  return _activeBank;
}

//////////////////////////////////
// Normal Mode
NormalMode::NormalMode(midi_t& midi, const LightManager& lightManager, Screen** ppScreens, const BankMode& bankMode) :
  _midi(midi),
  _lightManager(lightManager),
  _ppScreens(ppScreens),
  _bankMode(bankMode),
  _activeButton(0)
{
}

void NormalMode::activate()
{
  PatchGlyph patchA("A", _activeButton == BUTTON_ONE-1);
  PatchGlyph patchB("B", _activeButton == BUTTON_TWO-1);
  PatchGlyph patchC("C", _activeButton == BUTTON_THREE-1);
  BankNumberGlyph bankNumber(_bankMode.getBank());

  _ppScreens[SCREEN_ONE]->draw(patchA);
  _ppScreens[SCREEN_TWO]->draw(patchB);
  _ppScreens[SCREEN_THREE]->draw(patchC);
  _ppScreens[SCREEN_FOUR]->draw(bankNumber);
    
  _lightManager.setFlashing(LIGHT_ONE, false);
  _lightManager.setFlashing(LIGHT_TWO, false);
  _lightManager.setFlashing(LIGHT_THREE, false);
  _lightManager.setFlashing(LIGHT_FOUR, false);
  
  _lightManager.turnAllOff();
  _lightManager.turnOn(_activeButton);  
  _lightManager.turnOn(LIGHT_FOUR);
  sendMidi();
}

void NormalMode::updateScreen(const byte number, const bool active)
{
  const char A = 'A';
  const char txt[2] = {(char)(A+number), 0u};
  PatchGlyph patch(txt, active);
  _ppScreens[number]->draw(patch);
}

void NormalMode::buttonPressed(const byte number)
{
  _lightManager.turnOff(_activeButton);
  updateScreen(_activeButton, false);

  _activeButton = number - 1;

  _lightManager.turnOn(_activeButton);
  updateScreen(_activeButton, true);
  sendMidi();  
}

void NormalMode::buttonLongPressed(const byte number)
{
}

void NormalMode::sendMidi() const
{
  // send the snap shot change to the stomp
  _midi.sendControlChange(STOMP_SNAPSHOT_CC, _activeButton, STOMP_MIDI_CHANNEL);

  // send program change to the amp
  const byte activeBank = _bankMode.getBank();
  byte patch = (activeBank * 3) + _activeButton;
  _midi.sendProgramChange(patch, AMP_MIDI_CHANNEL);
}

//////////////////////////////////
// Looper Mode
LooperMode::LooperMode(midi_t& midi, const LightManager& lightManager, Screen** ppScreens) :
  _midi(midi),
  _lightManager(lightManager),
  _ppScreens(ppScreens),
  _state(NO_LOOP),
  _loopGlyph(LoopGlyph())
{
}

void LooperMode::activate()
{
  SymbolGlyph play(PLAY);
  SymbolGlyph stop(STOP);
  SymbolGlyph record(RECORD);

  _ppScreens[SCREEN_ONE]->draw(play);
  _ppScreens[SCREEN_TWO]->draw(stop);
  _ppScreens[SCREEN_THREE]->draw(record);
  

  _lightManager.turnAllOff();
  _lightManager.turnOn(LIGHT_FOUR);
  _lightManager.setFlashing(LIGHT_FOUR, false);  
  
  this->updateState();
}

void LooperMode::updateState()
{
  _lightManager.turnOn(LIGHT_THREE);  

  switch(_state) {
    case NO_LOOP:
    {
      _lightManager.turnOff(LIGHT_ONE);
      _lightManager.turnOff(LIGHT_TWO);      
      _lightManager.setFlashing(LIGHT_THREE, false);

      const char* modeLines[2] = { "No", "Loop" };
      ModeGlyph mode(modeLines, 2);
      _ppScreens[SCREEN_FOUR]->draw(mode);
    } break;

    case STOPPED:
    {
      this->updateMode("Stopped");

      _lightManager.turnOn(LIGHT_ONE);
      _lightManager.setFlashing(LIGHT_ONE, false);
      _lightManager.turnOn(LIGHT_TWO);
      _lightManager.setFlashing(LIGHT_THREE, false);
      
    } break;

    case RECORDING:
    case OVER_DUBBING:
    {      
      this->updateMode("Recording");

      _lightManager.turnOn(LIGHT_ONE);
      _lightManager.setFlashing(LIGHT_ONE, true);
      _lightManager.turnOn(LIGHT_TWO);
      _lightManager.setFlashing(LIGHT_THREE, true);
    } break;

    case PLAYING:
    {
      this->updateMode("Playing");
      _lightManager.turnOn(LIGHT_ONE);
      _lightManager.setFlashing(LIGHT_ONE, true);
      _lightManager.turnOn(LIGHT_TWO);
      _lightManager.setFlashing(LIGHT_THREE, false);
    } break;

  }
}

void LooperMode::updateMode(const char* line)
{
  _loopGlyph.setCaption(line);  
  _ppScreens[SCREEN_FOUR]->draw(_loopGlyph);
}

void LooperMode::buttonPressed(const byte number)
{  
  LooperState newState = _state;

  switch(number) 
  {
    case BUTTON_ONE:
      newState = this->play();
    break;

    case BUTTON_TWO:
      newState = this->stop();
    break;

    case BUTTON_THREE:
      newState = this->record();
    break;
  }

  // only update if we're changing state
  if(newState != _state)
  {
    _state = newState;
    this->updateState();
  }
}

void LooperMode::buttonLongPressed(const byte number)
{  
  LooperState newState = _state;

  switch(number) 
  {
    case BUTTON_TWO:
      newState = this->longStop();
    break;

    case BUTTON_THREE:
      newState = this->longRecord();
    break;

  }

  // only update if we're changing state
  if(newState != _state)
  {
    _state = newState;
    this->updateState();
  }
}

LooperState LooperMode::play()
{
  switch(_state) {
    case STOPPED:
    case RECORDING:
    case OVER_DUBBING:
    {
      _midi.sendControlChange(STOMP_LOOP_PLAY_STOP_CC, STOMP_LOOP_PLAY_VAL, STOMP_MIDI_CHANNEL);
      return PLAYING;
    } break;
  }

  return _state;
}

LooperState LooperMode::stop()
{
  switch(_state) {
    case RECORDING:
    case OVER_DUBBING:
    case PLAYING:
    {
      _midi.sendControlChange(STOMP_LOOP_PLAY_STOP_CC, STOMP_LOOP_STOP_VAL, STOMP_MIDI_CHANNEL);
      return STOPPED;
    } break;
  }

  return _state;
}

LooperState LooperMode::longStop()
{
  switch(_state) {
    case STOPPED:
      return NO_LOOP;
    break;    
  }

  return _state;
}

LooperState LooperMode::record()
{
  switch(_state) {
    case NO_LOOP:
    {
      _midi.sendControlChange(STOMP_LOOP_RECORD_OVERDUB_CC, STOMP_LOOP_RECORD_VAL, STOMP_MIDI_CHANNEL);
      return RECORDING;
    } break;

    case STOPPED:
    case PLAYING:
    case RECORDING:
    {
      _midi.sendControlChange(STOMP_LOOP_RECORD_OVERDUB_CC, STOMP_LOOP_OVERDUB_VAL, STOMP_MIDI_CHANNEL);
      return OVER_DUBBING;
    } break;

    case OVER_DUBBING:
    {
      _midi.sendControlChange(STOMP_LOOP_PLAY_STOP_CC, STOMP_LOOP_PLAY_VAL, STOMP_MIDI_CHANNEL);
      return PLAYING;
    } break;
  }

  return _state;
}

LooperState LooperMode::longRecord()
{
  switch(_state) {
    case PLAYING:
    { 
      _midi.sendControlChange(STOMP_LOOP_UNDO_REDO_CC, STOMP_LOOP_UNDO_REDO_VAL, STOMP_MIDI_CHANNEL);
    } break;    
  }

  return _state;
}

//////////////////////////////////
// Mode Manager
ModeManager::ModeManager(IMode** ppModes, const byte nModes) :
  _ppModes(ppModes),
  _nModes(nModes),
  _currentMode(0)
{
}

IMode& ModeManager::getMode() const
{
  return *_ppModes[_currentMode];
}

void ModeManager::buttonPressed(const byte number)
{
  _currentMode = _currentMode != NORMAL_MODE ? NORMAL_MODE : BANK_MODE;
  _ppModes[_currentMode]->activate();  
}

void ModeManager::buttonLongPressed(const byte number)
{
  if(_currentMode != LOOPER_MODE)
  {
    _currentMode = LOOPER_MODE;
    _ppModes[_currentMode]->activate();  
  }
}
