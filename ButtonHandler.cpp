
#include <Arduino.h>
#include "ButtonHandler.h"

ButtonHandler::ButtonHandler()
{
}

ButtonHandler::ButtonHandler(const ButtonHandler& rhs)
{
}

ButtonHandler::ButtonHandler(const ModeManager& modeManager) :
  _modeManager(modeManager)
{
}

const void ButtonHandler::buttonPressed(const byte& number)
{
  const IMode& mode = _modeManager.getMode();
  mode.buttonPressed(number);
}
