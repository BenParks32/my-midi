#include "Button.h"

Button::Button()
{
}

Button::Button(const Button& rhs)
{
}

Button::Button(const byte& number, const byte& pin, const IButtonDelegate& delegate) :
  _debouncer(Bounce()),
  _number(number),
  _pin(pin),
  _delegate(delegate),
  _state(IS_OPEN)
{
  pinMode(_pin, INPUT_PULLUP);
  _debouncer.attach(_pin);
  _debouncer.interval(5);
}

const void Button::updateState()
{
  _debouncer.update();
  byte pinIs = _debouncer.read(); 
 
  switch (_state)
  {
    case IS_OPEN:
      if (pinIs == HIGH)
        _state = IS_RISING;
      break;

    case IS_RISING:
      _state = IS_CLOSED;
      break;

    case IS_CLOSED:
      if (pinIs == LOW)
        _state = IS_FALLING;
      break;

    case IS_FALLING:
      _delegate.buttonPressed(_number);
      _state = IS_OPEN;
      break;
  }
}
