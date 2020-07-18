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
      if (millis() - _chrono > 750) 
      {
      	_delegate.buttonLongPressed(_number);        
        _longPressed = true;
      }

      if (pinIs == HIGH)
      {
        _state = IS_RISING;
      }      
    } break;

    case IS_RISING:
    {
      _state = IS_CLOSED;

      if(!_longPressed)      
      {
        _delegate.buttonPressed(_number);
      }

      _longPressed = false;
      _chrono = 0;
      
    } break;

    case IS_CLOSED:
    {
      if (pinIs == LOW)
      {
        _state = IS_FALLING;
      }
    } break;

    case IS_FALLING:
    {     
      _chrono = millis(); 
      _state = IS_OPEN;
    } break;
  }
}
