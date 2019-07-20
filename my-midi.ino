
#include "Button.h"
#include "ButtonHandler.h"
#include "Mode.h"
#include "Light.h"
#include "LightManager.h"

const byte MODE_COUNT = 2;
const byte BUTTON_COUNT = 4;

const Light led1(1, 3);
const Light led2(2, 5);
const Light led3(3, 7);
const Light led4(4, 9);
Light *leds[] {&led1, &led2, &led3, &led4};

LightManager lightManager(leds, BUTTON_COUNT);

PatchMode patchMode(lightManager);
NormalMode normalMode(lightManager, patchMode);

IMode *modes[] {&normalMode, &patchMode};

const ModeManager modeManager(modes, MODE_COUNT);
const ButtonHandler buttonHandler(modeManager);

const Button btn1(1, 2, buttonHandler);
const Button btn2(2, 4, buttonHandler);
const Button btn3(3, 6, buttonHandler);
const Button btn4(4, 8, modeManager);
Button *buttons[] {&btn1, &btn2, &btn3, &btn4};

void setup() {
  Serial.begin(9600);
  normalMode.activate();
}

void loop() {
 
  for(int i=0; i < BUTTON_COUNT; ++i)
  {
    buttons[i]->updateState();
  }

  lightManager.updateLights();
}
