#ifndef _mode_h
#define _mode_h

#include <MIDI.h>

#include "LightManager.h"
#include "Button.h"
#include "Screen.h"

typedef midi::MidiInterface<HardwareSerial> midi_t;

class IMode
{
  public:
    virtual void activate() = 0;
    virtual void buttonPressed(const byte number) = 0;
};

class BankMode : public IMode
{
  public:
    BankMode(const midi_t& midi, const LightManager& lightManager, const Screen** ppScreens);

  private:
    BankMode();
    BankMode(const BankMode& rhs);

  public:
    virtual void activate();
    virtual void buttonPressed(const byte number);

    byte getBank() const;

  private:
    const midi_t& _midi;
    const LightManager& _lightManager;
    const Screen** _ppScreens;
    byte _activeBank;
};

class NormalMode : public IMode
{
  public:
    NormalMode(const midi_t& midi, const LightManager& lightManager, const Screen** ppScreens, const BankMode& bankMode);

  private:
    NormalMode();
    NormalMode(const NormalMode& rhs);

  public:
    virtual void activate();
    virtual void buttonPressed(const byte number);

  private:
    void sendMidi() const;

  private:
    const midi_t& _midi;
    const LightManager& _lightManager;
    const Screen** _ppScreens;
    const BankMode& _bankMode;
    byte _activeButton;
};

class ModeManager : public IButtonDelegate
{
  public:
    ModeManager(IMode** ppModes, const byte nModes);
 
  private:    
    ModeManager();
    ModeManager(const ModeManager& rhs);

  public:
    virtual void buttonPressed(const byte number);
    IMode& getMode() const;

  private:
    IMode** _ppModes;
    const byte _nModes;
    byte _currentMode;   
};

#endif
