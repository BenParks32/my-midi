
#include "Screen.h"

const byte CLOCK = 13;
const byte DATA = 11;
const byte DC = 10;

//////////////////////////////////
// TextGlyph
PatchGlyph::PatchGlyph(const char* txt, bool active) :
  _txt(txt),
  _active(active)
{
}

void PatchGlyph::draw(screen_t& ctx)
{
  XY pos(47, 57);

  ctx.firstPage();
  ctx.setFont(u8g2_font_logisoso50_tr);
  do {
    ctx.drawStr(pos.x, pos.y, _txt);
    if(_active)
    {
      ctx.drawFrame(34, 0, 60, 64);
      ctx.drawFrame(35, 1, 58, 62);
    }
  } while ( ctx.nextPage() );
}
//////////////////////////////////
// BankNumberGlyph
BankNumberGlyph::BankNumberGlyph(const byte bank) :
  _bank(bank)
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
    ctx.drawStr(40, 22, "Bank");
    ctx.setFont(u8g2_font_logisoso38_tr);
    ctx.drawStr(41, 63, num);

  } while ( ctx.nextPage() );
}
//////////////////////////////////
// SymbolGlyph
SymbolGlyph::SymbolGlyph(const SYMBOL_TYPE type) :
  _type(type)
{
}

void SymbolGlyph::draw(screen_t& ctx)
{
  const XY pos(42, 10);
  const SZ sz(42, 42);

  ctx.firstPage();
  do {
    switch (_type)
    {
      case ARROW_DOWN:
      case ARROW_UP:
        this->drawArrow(ctx, pos, sz);
        break;

      case PLAY:
        this->drawPlay(ctx, pos, sz);
        break;

      case STOP:
        this->drawStop(ctx, pos, sz);
        break;        
 
      case RECORD:
        this->drawRecord(ctx, pos, sz);
        break;        
    }
  } while ( ctx.nextPage() );
}

void SymbolGlyph::drawArrow(screen_t& ctx, const XY& pos, const SZ& sz)
{
  const byte w2 = sz.w / 2;
  const byte w4 = sz.w / 4;
  const byte bh = 10;

  switch (_type)
    {
      case ARROW_DOWN:
        ctx.drawTriangle(pos.x + w2, pos.y, pos.x, pos.y + sz.h - bh, pos.x + sz.w, pos.y + sz.h - bh);
        ctx.drawBox(pos.x + w4, pos.y + sz.h - bh, w2 + 2, bh);
        break;

      case ARROW_UP:
        ctx.drawTriangle(pos.x, pos.y + bh, pos.x + sz.w, pos.y + bh, pos.x + w2, pos.y + sz.h);
        ctx.drawBox(pos.x + w4, pos.y, w2 + 2, bh);
        break;
    }
}

void SymbolGlyph::drawPlay(screen_t& ctx,  const XY& pos, const SZ& sz)
{
  const byte h2 = sz.h / 2;
  ctx.drawTriangle(pos.x, pos.y, pos.x + sz.w, pos.y + h2, pos.x, pos.y + sz.h);
}

void SymbolGlyph::drawStop(screen_t& ctx,  const XY& pos, const SZ& sz)
{
  ctx.drawBox(pos.x, pos.y, sz.w, sz.h);
}

void SymbolGlyph::drawRecord(screen_t& ctx,  const XY& pos, const SZ& sz)
{
  const byte rad = sz.w/2;
  ctx.drawDisc(pos.x + rad, pos.y + rad, rad);
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
// LoopGlyph
LoopGlyph::LoopGlyph() :
  _caption(NULL),
  _startTime(0),
  _loopLength(0),
  _chrono(millis())
{
}

void LoopGlyph::setStartTime(long startTime)
{
  _startTime = startTime;  
}

void LoopGlyph::setLoopLength(long loopLength)
{
  _loopLength = loopLength;  
}

void LoopGlyph::setCaption(const char* caption)
{
  _caption = caption;
  _repaint = true;
}

void LoopGlyph::draw(screen_t& ctx)
{
  const long now = millis();
  if(!_repaint && now - _chrono < 1000)
    return;

  _chrono = now;
  _repaint = false;

  const long time = ((now - _startTime) % _loopLength);
  const int width = 108 * (time/(double)_loopLength);

  ctx.firstPage();
  ctx.setFont(u8g2_font_logisoso20_tr); 

  do {
    ctx.drawStr(10, 25, _caption);
    ctx.drawBox(10, 37, width, 20);
    ctx.drawFrame(10+width, 37, 108-width, 20);
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
