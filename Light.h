#ifndef _light_h
#define _light_h

class Light
{
  public:
    Light(const byte& number, const byte& pin);

  private:
    Light();
    Light(const Light& rhs);

  public:
    const void turnOn() const;
    const void turnOn(bool on) const;
    const void turnOff() const;
    const void setFlashing(const bool flashing);
    const bool isFlashing() const;

  private:
    const byte _number;
    const byte _pin;
    bool _flashing;
};

#endif
