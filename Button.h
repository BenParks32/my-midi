#ifndef button_h
#define button_h

#include <Arduino.h>
#include <Bounce2.h>

class IButtonDelegate
{
  public:
    virtual const void buttonPressed(const byte& number) = 0;
};

enum ButtonState {IS_OPEN, IS_RISING, IS_CLOSED, IS_FALLING};

class Button
{
  public:
    Button(const byte& number, const byte& pin, const IButtonDelegate& delegate);

  private:
    Button();
    Button(const Button& rhs);

  public:
    const void init() const;
    const void updateState();

  private:
    const Bounce _debouncer;
    const byte _number;
    const byte _pin;
    const IButtonDelegate& _delegate;
    ButtonState _state;
};

#endif
