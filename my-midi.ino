#include <MIDI.h>

#include "Button.h"
#include "ButtonHandler.h"
#include "Mode.h"
#include "Light.h"
#include "LightManager.h"
#include "Screen.h"
#include "Store.h"

const byte MODE_COUNT = 3;
const byte BUTTON_COUNT = 4;

MIDI_CREATE_DEFAULT_INSTANCE();

Store store;

Light led1(1, 3);
Light led2(2, 5);
Light led3(3, 7);
Light led4(4, 9);
Light *leds[BUTTON_COUNT] {&led1, &led2, &led3, &led4};
LightManager lightManager(leds, BUTTON_COUNT);

Screen *screens[BUTTON_COUNT];

BankMode bankMode(MIDI, store, lightManager, screens);
NormalMode normalMode(MIDI, store, lightManager, screens);
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
  
  store.load();

  for (int i = 0; i < BUTTON_COUNT; ++i)
  {
    screens[i] = new Screen(A0+i);
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
