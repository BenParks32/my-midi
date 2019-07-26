#ifndef _screen_h
#define _screen_h

#include <SPI.h>
#include <U8g2lib.h>

typedef U8G2_SH1106_128X64_NONAME_1_4W_SW_SPI screen_t;

struct XY
{
  XY(const byte a, const byte b) : x(a), y(b) {}    
  const byte x;
  const byte y;
};

class IGlyph
{
  public:
    virtual void draw(const screen_t& ctx) const = 0;
};

class TextGlyph : public IGlyph
{
  public:
    TextGlyph(const char* txt, const uint8_t* font, const XY& pos);

  private:
    TextGlyph();
    TextGlyph(const TextGlyph& rhs);

  public:
    virtual void draw(const screen_t& ctx) const;

  private:
    const char* _txt;
    const uint8_t* _font;
    const XY _pos;
};

class PatchGlyph : public TextGlyph
{
  public:
    PatchGlyph(const char* letter);
};

class BankGlyph : public TextGlyph
{
  public:
    BankGlyph(const char* letter);
};

class BankUpGlyph : public BankGlyph
{
  public:
    BankUpGlyph();
};

class BankDownGlyph : public BankGlyph
{
  public:
    BankDownGlyph();
};

class Screen
{
  public:
    Screen(const byte pin);
  
  private:
    Screen();
    Screen(const Screen& rhs);

  public:
    void setup() const;
    void draw(const IGlyph& glyph) const;

  private:
    const screen_t _u8g2;
    const byte _pin;
};

#endif
