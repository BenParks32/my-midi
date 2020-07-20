#ifndef _screen_h
#define _screen_h

#include <SPI.h>
#include <U8g2lib.h>

typedef U8G2_SH1106_128X64_NONAME_1_4W_SW_SPI screen_t;

enum SYMBOL_TYPE {ARROW_UP, ARROW_DOWN, PLAY, STOP, RECORD};

struct XY
{
  XY(const byte a, const byte b) : x(a), y(b) {}
  XY(): x(0), y(0) {}
  byte x;
  byte y;
};

struct SZ
{
  SZ(const byte a, const byte b) : w(a), h(b) {}
  const byte w;
  const byte h;
};

class IGlyph
{
  public:
    virtual void draw(screen_t& ctx) = 0;
};

class PatchGlyph : public IGlyph
{
  public:
    PatchGlyph(const char* txt, const bool active);

  private:
    PatchGlyph();
    PatchGlyph(const PatchGlyph& rhs);

  public:
    virtual void draw(screen_t& ctx);

  private:
    const char* _txt;
    const bool _active;
};

class BankNumberGlyph : public IGlyph
{
  public:
    BankNumberGlyph(const byte bank);

  public:
    virtual void draw(screen_t& ctx);

  private:
    byte _bank;
};

class SymbolGlyph : public IGlyph
{
  public:
    SymbolGlyph(const SYMBOL_TYPE type);

  public:
    virtual void draw(screen_t& ctx);

  private:
    void drawArrow(screen_t& ctx, const XY& pos, const SZ& sz);
    void drawPlay(screen_t& ctx,  const XY& pos, const SZ& sz);
    void drawStop(screen_t& ctx,  const XY& pos, const SZ& sz);
    void drawRecord(screen_t& ctx,  const XY& pos, const SZ& sz);

  private:
    const SYMBOL_TYPE _type;
};

class ModeGlyph : public IGlyph
{
  public:
    ModeGlyph(const char** pLines, const byte numLines);

  public:
    virtual void draw(screen_t& ctx);

  private:
    const char** _pLines;
    const byte _numLines;
};

class LoopGlyph : public IGlyph
{
  public:
    LoopGlyph();

  public:
    virtual void draw(screen_t& ctx);
    void setCaption(const char* caption);
    void setTime(short newTime);

  private:
    const char* _caption;
    const short _length;
    short _time;
};

class Screen
{
  public:
    Screen(const byte pin);

  private:
    Screen();
    Screen(const Screen& rhs);

  public:
    void setup();
    void draw(IGlyph& glyph);
    void clear();

  private:
    screen_t _u8g2;
    const byte _pin;
};

#endif
