
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

//////////////////////////////////
// Bank Mode
BankMode::BankMode(const LightManager& lightManager) :
  _lightManager(lightManager),
  _activeBank(0)
{
}

BankMode::BankMode() :
  _lightManager(LightManager(0, 0)),
  _activeBank(0)
{
}

BankMode::BankMode(const BankMode& rhs) :
  _lightManager(rhs._lightManager),
  _activeBank(rhs._activeBank)
{
}

void BankMode::activate() const
{
  _lightManager.turnAllOff();
  _lightManager.turnOn(LIGHT_ONE);  
  _lightManager.turnOn(LIGHT_THREE);
  _lightManager.setFlashing(LIGHT_FOUR, true);
}

void BankMode::buttonPressed(const byte number)
{
  switch(number)
  {
    case BUTTON_ONE:
    {
      if(_activeBank < MAX_PATCH)
        ++_activeBank;
      else
        _activeBank = 0;
    } break;

    case BUTTON_THREE:
    {
      if(_activeBank > 0)
        --_activeBank;
      else
        _activeBank = MAX_PATCH;
    } break;

    default:
      break;
  } 
}

byte BankMode::getBank() const
{
  return _activeBank;
}

//////////////////////////////////
// Normal Mode
NormalMode::NormalMode(const LightManager& lightManager, const BankMode& bankMode) :
  _lightManager(lightManager),
  _bankMode(bankMode),
  _activeButton(0)
{
}

NormalMode::NormalMode() :
  _lightManager(LightManager(0,0)),
  _bankMode(BankMode(LightManager(0,0))),
  _activeButton(0)
{
}

NormalMode::NormalMode(const NormalMode& rhs) :
  _lightManager(rhs._lightManager),
  _bankMode(rhs._bankMode),
  _activeButton(rhs._activeButton)
{
}

void NormalMode::activate() const
{
  _lightManager.setFlashing(LIGHT_FOUR, false);
  
  _lightManager.turnAllOff();
  _lightManager.turnOn(_activeButton);  
  _lightManager.turnOn(LIGHT_FOUR);
}

void NormalMode::buttonPressed(const byte number)
{
  _lightManager.turnOff(_activeButton);
  _activeButton = number - 1;
  _lightManager.turnOn(_activeButton);
  
  // Serial.print("Send Stomp SS CC 69 ");
  // Serial.println(_activeButton);

  const byte activeBank = _bankMode.getBank();
  // Serial.print("Send Amp PC ");
  // Serial.println((activeBank * 3) + _activeButton);
}

//////////////////////////////////
// Mode Manager
ModeManager::ModeManager(IMode** ppModes, const byte nModes) :
  _ppModes(ppModes),
  _nModes(nModes),
  _currentMode(0)
{
}

ModeManager::ModeManager():
  _ppModes(0),
  _nModes(0),
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
  {
    ++_currentMode;
  }
  else
  {
    _currentMode = 0;
  }
  _ppModes[_currentMode]->activate();  
}
