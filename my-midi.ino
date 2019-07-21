#include <MIDI.h>

#include "Button.h"
#include "ButtonHandler.h"
#include "Mode.h"
#include "Light.h"
#include "LightManager.h"

const byte MODE_COUNT = 2;
const byte BUTTON_COUNT = 4;

MIDI_CREATE_DEFAULT_INSTANCE();

Light led1(1, 3);
Light led2(2, 5);
Light led3(3, 7);
Light led4(4, 9);
Light *leds[BUTTON_COUNT] {&led1, &led2, &led3, &led4};
LightManager lightManager(leds, BUTTON_COUNT);

BankMode bankMode(MIDI, lightManager);
NormalMode normalMode(MIDI, lightManager, bankMode);
IMode *modes[MODE_COUNT] {&normalMode, &bankMode};

const ModeManager modeManager(modes, MODE_COUNT);
const ButtonHandler buttonHandler(modeManager);

Button btn1(1, 2, buttonHandler);
Button btn2(2, 4, buttonHandler);
Button btn3(3, 6, buttonHandler);
Button btn4(4, 8, modeManager);
Button *buttons[BUTTON_COUNT] {&btn1, &btn2, &btn3, &btn4};

void setup() {    
  normalMode.activate();
  MIDI.begin(MIDI_CHANNEL_OFF);
}

void loop() {
 
  for(int i=0; i < BUTTON_COUNT; ++i)
  {
    buttons[i]->updateState();
  }

  lightManager.updateLights();
}
