#include <MIDI.h>

#include "Button.h"
#include "ButtonHandler.h"
#include "Mode.h"
#include "Light.h"
#include "LightManager.h"
#include "Screen.h"

const byte MODE_COUNT = 3;
const byte BUTTON_COUNT = 4;

MIDI_CREATE_DEFAULT_INSTANCE();

Light led1(1, 3);
Light led2(2, 5);
Light led3(3, 7);
Light led4(4, 9);
Light *leds[BUTTON_COUNT] {&led1, &led2, &led3, &led4};
LightManager lightManager(leds, BUTTON_COUNT);

// Screen scr1(A0);
// Screen scr2(A1);
// Screen scr3(A2);
// Screen scr4(A3);
// Screen *screens[BUTTON_COUNT] {&scr1, &scr2, &scr3, &scr4};

Screen *screens[BUTTON_COUNT];

BankMode bankMode(MIDI, lightManager, screens);
NormalMode normalMode(MIDI, lightManager, screens, bankMode);
LooperMode looperMode(MIDI, lightManager, screens);
IMode *modes[MODE_COUNT] {&normalMode, &bankMode, &looperMode};

ModeManager modeManager(modes, MODE_COUNT);
ButtonHandler buttonHandler(modeManager);

Button btn1(1, 12, buttonHandler); // using 12 as 2 seems to be broken!
Button btn2(2, 4, buttonHandler);
Button btn3(3, 6, buttonHandler);
Button btn4(4, 8, modeManager);
Button *buttons[BUTTON_COUNT] {&btn1, &btn2, &btn3, &btn4};

void setup() {
  
  for (int i = 0; i < BUTTON_COUNT; ++i)
  {
    screens[i] = new Screen(A0+i);
    screens[i]->setup();
  }

  // does this fix the faded screens on cold startup?
  for (int i = 0; i < BUTTON_COUNT; ++i)
  {    
    screens[i]->setup();
  }
  
  MIDI.begin(MIDI_CHANNEL_OFF);  
  normalMode.activate();  
}

void loop() {

  for (int i = 0; i < BUTTON_COUNT; ++i)
  {
    buttons[i]->updateState();
  }

  lightManager.updateLights();
  modeManager.getMode().frameTick();
}
