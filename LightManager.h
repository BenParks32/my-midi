#ifndef _light_manager_h
#define _light_manager_h

#include "Light.h"

class LightManager
{
  public:
    LightManager(Light** ppLights, const byte& nLights);

  private:
    LightManager();
    LightManager(const LightManager& rhs);

  public:
    const void turnOn(const byte& number, const bool on) const;
    const void turnOn(const byte& number) const;
    const void turnOff(const byte& number) const;
    const void turnAllOff() const;
    const void setFlashing(const byte& number, const bool flashing) const;
    const void updateLights();

  private:
    const Light** _ppLights;
    const byte _nLights;

    long _chrono;
    bool _flashOn;
};

#endif
