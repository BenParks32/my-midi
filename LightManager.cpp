
#include <Arduino.h>
#include "LightManager.h"

LightManager::LightManager(Light** ppLights, const byte nLights) :
  _ppLights(ppLights),
  _nLights(nLights),
  _chrono(millis())
{
}

void LightManager::turnOn(const byte number, const bool on) const
{
  if (on)
    turnOn(number);
  else
    turnOff(number);
}

void LightManager::turnOn(const byte number) const
{
  _ppLights[number]->turnOn();
}

void LightManager::turnOff(const byte number) const
{
  _ppLights[number]->turnOff();
}

void LightManager::turnAllOff() const
{
  for (int i = 0; i < _nLights; ++i)
  {
    _ppLights[i]->turnOff();
  }
}

void LightManager::setFlashing(const byte number, const bool flashing) const
{
  _ppLights[number]->setFlashing(flashing);
}

void LightManager::updateLights()
{
  long now = millis();
  if (now - _chrono > 250)
  {
    _chrono = now;
    _flashOn = !_flashOn;
    for (int i = 0; i < _nLights; ++i)
    {
      if (_ppLights[i]->isFlashing())
      {
        _ppLights[i]->turnOn(_flashOn);
      }
    }
  }
}
