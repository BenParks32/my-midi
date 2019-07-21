
#include <Arduino.h>
#include "ButtonHandler.h"

ButtonHandler::ButtonHandler(const ModeManager& modeManager) :
  _modeManager(modeManager)
{
}

ButtonHandler::ButtonHandler() :
  _modeManager(ModeManager(0, 0))
{
}

ButtonHandler::ButtonHandler(const ButtonHandler& rhs):
  _modeManager(rhs._modeManager)
{
}

void ButtonHandler::buttonPressed(const byte number)
{
  IMode& mode = _modeManager.getMode();
  mode.buttonPressed(number);
}
