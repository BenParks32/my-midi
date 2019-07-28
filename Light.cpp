
#include <Arduino.h>
#include "Light.h"

Light::Light(const byte number, const byte pin) :
  _number(number),
  _pin(pin),
  _flashing(false)
{
  pinMode(_pin, OUTPUT);
}

void Light::turnOn(bool on) const
{
  if(on)
    turnOn();
  else
    turnOff();  
}

void Light::turnOn() const
{
  digitalWrite(_pin, HIGH);
}

void Light::turnOff() const
{  
  digitalWrite(_pin, LOW);
}

void Light::setFlashing(const bool flashing)
{
  _flashing = flashing;
}

bool Light::isFlashing() const
{
  return _flashing;
}
