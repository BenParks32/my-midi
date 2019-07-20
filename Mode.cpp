
#include <Arduino.h>
#include "Mode.h"

const byte MAX_PATCH = 39;

//////////////////////////////////
// Patch Mode
PatchMode::PatchMode(const LightManager& lightManager) :
  _lightManager(lightManager),
  _activePatch(0)
{
}

PatchMode::PatchMode()
{
}

PatchMode::PatchMode(const PatchMode& rhs)
{
}

const char* PatchMode::getName() const
{
  return "PatchMode";
}

const void PatchMode::activate() const
{
  _lightManager.turnAllOff();
  _lightManager.turnOn(0);  
  _lightManager.turnOn(2);
  _lightManager.setFlashing(3, true);
}

const void PatchMode::buttonPressed(const byte& number)
{
  switch(number)
  {
    case 1:
    {
      Serial.println("Patch up");
      if(_activePatch < MAX_PATCH)
        ++_activePatch;
      else
        _activePatch = 0;
    } break;

    case 3:
    {
      Serial.println("Patch down");
      if(_activePatch > 0)
        --_activePatch;
      else
        _activePatch = MAX_PATCH;
    } break;

    default:
      Serial.println("Do nothing!");
      break;
  } 

  Serial.print("New patch is ");
  Serial.println(_activePatch);
  Serial.print("Send Stomp PC ");
  Serial.println(_activePatch);
}

const byte PatchMode::getPatch() const
{
  return _activePatch;
}

//////////////////////////////////
// Normal Mode
NormalMode::NormalMode(const LightManager& lightManager, const PatchMode& patchMode) :
  _lightManager(lightManager),
  _patchMode(patchMode),
  _activeButton(0)
{
}

NormalMode::NormalMode()
{
}

NormalMode::NormalMode(const NormalMode& rhs)
{
}

const char* NormalMode::getName() const
{
  return "NormalMode";
}

const void NormalMode::activate() const
{
  _lightManager.setFlashing(3, false);
  
  _lightManager.turnAllOff();
  _lightManager.turnOn(_activeButton);  
  _lightManager.turnOn(3);
}

const void NormalMode::buttonPressed(const byte& number)
{
  _lightManager.turnOff(_activeButton);
  _activeButton = number - 1;
  _lightManager.turnOn(_activeButton);
  
  Serial.print("Send Stomp SS CC 69 ");
  Serial.println(_activeButton);

  const byte activePatch = _patchMode.getPatch();
  Serial.print("Send Amp PC ");
  Serial.println((activePatch * 3) + _activeButton);
}

//////////////////////////////////
// Mode Manager
ModeManager::ModeManager()
{
}

ModeManager::ModeManager(const ModeManager& rhs)
{
}

ModeManager::ModeManager(IMode** ppModes, const byte& nModes) :
  _ppModes(ppModes),
  _nModes(nModes),
  _currentMode(0)
{
}

const IMode& ModeManager::getMode() const
{
  return *_ppModes[_currentMode];
}

const void ModeManager::buttonPressed(const byte& number)
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
  Serial.print("New mode is ");
  Serial.println(_ppModes[_currentMode]->getName());
}
