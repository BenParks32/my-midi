#ifndef _mode_h
#define _mode_h

#include "Button.h"
#include "LightManager.h"

class IMode
{
  public:
    virtual void activate() const = 0;
    virtual void buttonPressed(const byte number) = 0;
};

class BankMode : public IMode
{
  public:
    BankMode(const LightManager& lightManager);

  private:
    BankMode();
    BankMode(const BankMode& rhs);

  public:
    virtual void activate() const;
    virtual void buttonPressed(const byte number);

    byte getBank() const;

  private:
    const LightManager& _lightManager;
    byte _activeBank;
};

class NormalMode : public IMode
{
  public:
    NormalMode(const LightManager& lightManager, const BankMode& bankMode);

  private:
    NormalMode();
    NormalMode(const NormalMode& rhs);

  public:
    virtual void activate() const;
    virtual void buttonPressed(const byte number);

  private:
    const LightManager& _lightManager;
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
