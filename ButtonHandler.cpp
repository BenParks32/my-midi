
#include <Arduino.h>
#include "ButtonHandler.h"

ButtonHandler::ButtonHandler(const ModeManager& modeManager) :
  _modeManager(modeManager)
{
}

void ButtonHandler::buttonPressed(const byte number)
{
  IMode& mode = _modeManager.getMode();
  mode.buttonPressed(number);
}
