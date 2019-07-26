
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
// Specific glyphs
PatchGlyph::PatchGlyph(const char* letter) : TextGlyph(letter, u8g2_font_logisoso58_tf, XY(44, 62)){}
BankGlyph::BankGlyph(const char* arrow) : TextGlyph(arrow, u8g2_font_open_iconic_arrow_8x_t, XY(32, 70)){}
BankUpGlyph::BankUpGlyph() : BankGlyph("S"){}
BankDownGlyph::BankDownGlyph() : BankGlyph("P"){}
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
