#ifndef button_h
#define button_h

#include <Arduino.h>
#include <Bounce2.h>

class IButtonDelegate
{
  public:
    virtual void buttonPressed(const byte number) = 0;
    virtual void buttonLongPressed(const byte number) = 0;
};

enum ButtonState {IS_OPEN, IS_RISING, IS_CLOSED, IS_FALLING};

class Button
{
  public:
    Button(const byte number, const byte pin, IButtonDelegate& delegate);

  private:
    Button();
    Button(const Button& rhs);

  public:
    void updateState();

  private:
    Bounce _debouncer;
    const byte _number;
    const byte _pin;
    IButtonDelegate& _delegate;
    ButtonState _state;
    long _chrono;
    bool _longPressing;
    bool _longPressed;
};

#endif
