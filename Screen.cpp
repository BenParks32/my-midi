
#include "Screen.h"

const byte CLOCK = 13;
const byte DATA = 11;
const byte DC = 10;

//////////////////////////////////
// TextGlyph
TextGlyph::TextGlyph(const char* txt, const uint8_t* font, const XY& pos) :
  _txt(txt),
  _font(font),
  _pos(pos)
{
}

void TextGlyph::draw(const screen_t& ctx) const
{
  ctx.firstPage();
  ctx.setFont(_font);
  do {
    ctx.drawStr(_pos.x, _pos.y, _txt);
  } while ( ctx.nextPage() );
}
//////////////////////////////////
// Text based glyphs
PatchGlyph::PatchGlyph(const char* letter) : TextGlyph(letter, u8g2_font_logisoso58_tr, XY(44, 62)) {}
BankGlyph::BankGlyph(const char* arrow) : TextGlyph(arrow, u8g2_font_open_iconic_arrow_8x_t, XY(32, 70)) {}
BankUpGlyph::BankUpGlyph() : BankGlyph("S") {}
BankDownGlyph::BankDownGlyph() : BankGlyph("P") {}
//////////////////////////////////
// BankNumberGlyph
BankNumberGlyph::BankNumberGlyph(const byte bank) :
  _bank(bank + 1)
{
}

void BankNumberGlyph::draw(const screen_t& ctx) const
{
  const char ZERO = 48;
  const char num[3] {ZERO + (_bank / 10), ZERO + (_bank % 10), 0};

  ctx.firstPage();
  do {
    ctx.setFont(u8g2_font_logisoso20_tr);
    ctx.drawStr(40, 22, "Bank");
    ctx.setFont(u8g2_font_logisoso38_tn);
    ctx.drawStr(41, 63, num);
  } while ( ctx.nextPage() );
}

//////////////////////////////////
// ModeGlyph
ModeGlyph::ModeGlyph(const char** pLines, const byte numLines) :
  _pLines(pLines),
  _numLines(numLines)
{
}

void ModeGlyph::draw(const screen_t& ctx) const
{
  const byte screenHeight = 64;
  const byte lineHeight = 25;
  const byte totalHeight = _numLines * lineHeight;
  const byte y = lineHeight + ((screenHeight - totalHeight) / 2);

  ctx.firstPage();
  ctx.setFont(u8g2_font_logisoso20_tr);
  do {
    for (byte line = 0; line < _numLines; ++line)
    {
      ctx.drawStr(30, y + (line * lineHeight), _pLines[line]);
    }
  } while ( ctx.nextPage() );
}

//////////////////////////////////
// Screen
Screen::Screen(const byte pin) :
  _u8g2(screen_t(U8G2_R0, CLOCK, DATA, pin, DC )),
  _pin(pin)
{
}

void Screen::setup() const
{
  pinMode(_pin, OUTPUT);
  _u8g2.begin();
}

void Screen::draw(const IGlyph& glyph) const
{
  glyph.draw(_u8g2);
}

void Screen::clear() const
{
  _u8g2.clearDisplay();
}
