#ifndef _light_manager_h
#define _light_manager_h

#include "Light.h"

class LightManager
{
  public:
    LightManager(Light** ppLights, const byte nLights);

  private:
    LightManager();
    LightManager(const LightManager& rhs);

  public:
    void turnOn(const byte number, const bool on) const;
    void turnOn(const byte number) const;
    void turnOff(const byte number) const;
    void turnAllOff() const;
    void setFlashing(const byte number, const bool flashing) const;
    void updateLights();

  private:
    Light** _ppLights;
    const byte _nLights;

    long _chrono;
    bool _flashOn;
};

#endif
