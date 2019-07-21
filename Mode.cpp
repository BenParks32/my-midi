
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

const byte STOMP_SNAPSHOT_CC = 69;
const byte STOMP_TUNER_CC = 68;

const byte STOMP_MIDI_CHANNEL = 1;
const byte AMP_MIDI_CHANNEL = 2;

//////////////////////////////////
// Bank Mode
BankMode::BankMode(const midi_t& midi, const LightManager& lightManager) :
  _midi(midi),
  _lightManager(lightManager),
  _activeBank(0),
  _tunerOn(false)
{
}

BankMode::BankMode()
{
}

BankMode::BankMode(const BankMode& rhs) :
  _midi(rhs._midi),
  _lightManager(rhs._lightManager),
  _activeBank(rhs._activeBank),
  _tunerOn(rhs._tunerOn)
{
}

void BankMode::activate()
{
  _lightManager.turnAllOff();
  _lightManager.turnOn(LIGHT_ONE);  
  _lightManager.turnOn(LIGHT_THREE);
  _lightManager.setFlashing(LIGHT_FOUR, true);
  _tunerOn = false;
}

void BankMode::buttonPressed(const byte number)
{
  switch(number)
  {
    case BUTTON_ONE:
    {
      if(_activeBank > 0)
        --_activeBank;
      else
        _activeBank = MAX_PATCH;
    } break;

    /*
    case BUTTON_TWO:
    {
      _midi.sendControlChange(STOMP_TUNER_CC, 0, STOMP_MIDI_CHANNEL);
      _tunerOn = !_tunerOn;
      _lightManager.turnOn(LIGHT_TWO, _tunerOn);
    } break;
    */
    
    case BUTTON_THREE:
    {
      if(_activeBank < MAX_PATCH)
        ++_activeBank;
      else
        _activeBank = 0;
    } break;

    default:
      break;
  }

  _midi.sendProgramChange(_activeBank, STOMP_MIDI_CHANNEL);
}

byte BankMode::getBank() const
{
  return _activeBank;
}

//////////////////////////////////
// Normal Mode
NormalMode::NormalMode(const midi_t& midi, const LightManager& lightManager, const BankMode& bankMode) :
  _midi(midi),
  _lightManager(lightManager),
  _bankMode(bankMode),
  _activeButton(0)
{
}

NormalMode::NormalMode()
{
}

NormalMode::NormalMode(const NormalMode& rhs) :
  _midi(rhs._midi),
  _lightManager(rhs._lightManager),
  _bankMode(rhs._bankMode),
  _activeButton(rhs._activeButton)
{
}

void NormalMode::activate()
{
  _lightManager.setFlashing(LIGHT_FOUR, false);
  
  _lightManager.turnAllOff();
  _lightManager.turnOn(_activeButton);  
  _lightManager.turnOn(LIGHT_FOUR);
  sendMidi();
}

void NormalMode::buttonPressed(const byte number)
{
  _lightManager.turnOff(_activeButton);
  _activeButton = number - 1;
  _lightManager.turnOn(_activeButton);
  sendMidi();  
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
// Mode Manager
ModeManager::ModeManager(IMode** ppModes, const byte nModes) :
  _ppModes(ppModes),
  _nModes(nModes),
  _currentMode(0)
{
}

ModeManager::ModeManager(const ModeManager& rhs):
  _ppModes(rhs._ppModes),
  _nModes(rhs._nModes),
  _currentMode(rhs._currentMode)
{
}

IMode& ModeManager::getMode() const
{
  return *_ppModes[_currentMode];
}

void ModeManager::buttonPressed(const byte number)
{
  if (_currentMode + 1 < _nModes)
    ++_currentMode;
  else
    _currentMode = 0;
    
  _ppModes[_currentMode]->activate();  
}
