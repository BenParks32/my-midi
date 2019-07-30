
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

void TextGlyph::draw(screen_t& ctx)
{
  ctx.firstPage();
  ctx.setFont(_font);
  do {
    ctx.drawStr(_pos.x, _pos.y, _txt);
  } while ( ctx.nextPage() );
}
//////////////////////////////////
// Text based glyphs
PatchGlyph::PatchGlyph(const char* letter) : TextGlyph(letter, u8g2_font_logisoso38_tr, XY(44, 62)) {}
//////////////////////////////////
// BankNumberGlyph
BankNumberGlyph::BankNumberGlyph(const byte bank) :
  _bank(bank + 1)
{
}

void BankNumberGlyph::draw(screen_t& ctx)
{
  const char ZERO = 48;  
  const char num1 = (char)(ZERO + (_bank / 10u));
  const char num2 = (char)(ZERO + (_bank % 10u));
  const char num[3] {num1, num2, 0u};

  ctx.firstPage();
  do {

    ctx.setFont(u8g2_font_logisoso20_tr);
    ctx.drawStr(40, 22, "Banks");
    ctx.setFont(u8g2_font_logisoso38_tr);
    ctx.drawStr(41, 63, num);

  } while ( ctx.nextPage() );
}
//////////////////////////////////
// ArrowUpGlyph
ArrowGlyph::ArrowGlyph(const ARROW_TYPE type) :
  _type(type)
{
}

void ArrowGlyph::draw(screen_t& ctx)
{
  const XY pos(42, 10);
  const SZ sz(42, 32);
  const byte w2 = sz.w / 2;
  const byte w4 = sz.w / 4;
  const byte bh = 10;

  ctx.firstPage();
  do {
    switch (_type)
    {
      case ARROW_DOWN:
        ctx.drawTriangle(pos.x, pos.y + bh, pos.x + sz.w, pos.y + bh, pos.x + w2, pos.y + sz.h + bh);
        ctx.drawBox(pos.x + w4, pos.y, w2 + 2, bh);
        break;

      case ARROW_UP:
        ctx.drawTriangle(pos.x + w2, pos.y, pos.x, pos.y + sz.h, pos.x + sz.w, pos.y + sz.h);
        ctx.drawBox(pos.x + w4, pos.y + sz.h, w2 + 2, bh);
        break;
    }
  } while ( ctx.nextPage() );
}
//////////////////////////////////
// ModeGlyph
ModeGlyph::ModeGlyph(const char** pLines, const byte numLines) :
  _pLines(pLines),
  _numLines(numLines)
{
}

void ModeGlyph::draw(screen_t& ctx)
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

void Screen::setup()
{
  pinMode(_pin, OUTPUT);
  _u8g2.begin();
}

void Screen::draw(IGlyph & glyph)
{
  glyph.draw(_u8g2);
}

void Screen::clear()
{
  _u8g2.clearDisplay();
}
