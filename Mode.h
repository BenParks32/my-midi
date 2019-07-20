#ifndef _mode_h
#define _mode_h

#include "Button.h"
#include "LightManager.h"

class IMode
{
  public:
    virtual char const* getName() const = 0;
    virtual const void activate() const = 0;
    virtual const void buttonPressed(const byte& number) = 0;
};

class PatchMode : public IMode
{
  public:
    PatchMode(const LightManager& lightManager);

  private:
    PatchMode();
    PatchMode(const PatchMode& rhs);

  public:
    virtual char const* getName() const;
    virtual const void activate() const;
    virtual const void buttonPressed(const byte& number);

    const byte getPatch() const;

  private:
    const LightManager& _lightManager;
    byte _activePatch;
};

class NormalMode : public IMode
{
  public:
    NormalMode(const LightManager& lightManager, const PatchMode& patchMode);

  private:
    NormalMode();
    NormalMode(const NormalMode& rhs);

  public:
    virtual char const* getName() const;
    virtual const void activate() const;
    virtual const void buttonPressed(const byte& number);

  private:
    const LightManager& _lightManager;
    const PatchMode& _patchMode;
    byte _activeButton;
};

class ModeManager : public IButtonDelegate
{
  public:
    ModeManager(IMode** ppModes, const byte& nModes);
 
  private:    
    ModeManager();
    ModeManager(const ModeManager& rhs);

  public:
    virtual const void buttonPressed(const byte& number);
    const IMode& getMode() const;

  private:
    const IMode** _ppModes;
    const byte _nModes;
    byte _currentMode;   
};

#endif
