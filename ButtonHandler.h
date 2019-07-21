#ifndef _midi_button_delegate_h
#define _midi_button_delegate_h

#include <Arduino.h>
#include "Button.h"
#include "Mode.h"

class ButtonHandler : public IButtonDelegate 
{
  public:
    ButtonHandler(const ModeManager& modeManager);
    virtual void buttonPressed(const byte pin);

  private:    
    ButtonHandler();
    ButtonHandler(const ButtonHandler& rhs);

  private:
    const ModeManager& _modeManager;
};

#endif
