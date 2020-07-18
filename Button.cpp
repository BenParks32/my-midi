#include "Button.h"

Button::Button(const byte number, const byte pin, IButtonDelegate& delegate) :
  _debouncer(Bounce()),
  _number(number),
  _pin(pin),
  _delegate(delegate),
  _state(IS_CLOSED),
  _chrono(0),
  _longPressing(false),
  _longPressed(false)
{
  pinMode(_pin, INPUT_PULLUP);
  _debouncer.attach(_pin);
  _debouncer.interval(5);
}

void Button::updateState()
{
  _debouncer.update();
  byte pinIs = _debouncer.read(); 
 
  switch (_state)
  {
    case IS_OPEN:
    {  
      if (millis() - _chrono > 1000) 
      {
        Serial.println("long pressed");
      	_delegate.buttonLongPressed(_number);        
        _longPressed = true;
      }

      if (pinIs == HIGH)
      {
        _state = IS_RISING;
      }
      Serial.println("open");
    } break;

    case IS_RISING:
    {
      _state = IS_CLOSED;
      _chrono = millis(); 
      Serial.println("closing");
    } break;

    case IS_CLOSED:
    {
      if (pinIs == LOW)
      {
        _state = IS_FALLING;
      }
      Serial.println("closed");
    } break;

    case IS_FALLING:
    {     
      if(!_longPressed)      
      {
        Serial.println("long pressed");
        _delegate.buttonPressed(_number);
      }

      _longPressed = false;
      _chrono = 0;
      _state = IS_OPEN;

      Serial.println("opening");

    } break;
  }
}
