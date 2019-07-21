
#include <Arduino.h>
#include "Light.h"

Light::Light(const byte number, const byte pin) :
  _number(number),
  _pin(pin),
  _flashing(false)
{
  pinMode(_pin, OUTPUT);
}

Light::Light() :
  _number(0),
  _pin(0),
  _flashing(false)
{ 
}

Light::Light(const Light& rhs) :
  _number(rhs._number),
  _pin(rhs._pin),
  _flashing(rhs._flashing)
{ 
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
