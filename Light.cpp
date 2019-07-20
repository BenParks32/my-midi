
#include <Arduino.h>
#include "Light.h"

Light::Light(const byte& number, const byte& pin) :
  _number(number),
  _pin(pin),
  _flashing(false)
{
  pinMode(_pin, OUTPUT);
}

Light::Light()
{ 
}

Light::Light(const Light& rhs)
{ 
}

const void Light::turnOn(bool on) const
{
  if(on)
    turnOn();
  else
    turnOff();  
}

const void Light::turnOn() const
{
  digitalWrite(_pin, HIGH);
}

const void Light::turnOff() const
{  
  digitalWrite(_pin, LOW);
}

const void Light::setFlashing(const bool flashing)
{
  _flashing = flashing;
}

const bool Light::isFlashing() const
{
  return _flashing;
}
