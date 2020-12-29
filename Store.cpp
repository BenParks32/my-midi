#include <Arduino.h>
#include <EEPROM.h>
#include "Store.h"

const int BANK_ADDRESS = 0;
const int PATCH_ADDRESS = 1;

Store::Store() :
 _bank(0),
 _patch(0)
{  
}

Store::Store(const Store& rhs) :
 _bank(rhs._bank),
 _patch(rhs._patch)
{  
}

void Store::load()
{
  _bank = EEPROM.read(BANK_ADDRESS);
  _bank = _bank >= MIN_BANK && _bank <= MAX_BANK ? _bank : MIN_BANK;

  _patch = EEPROM.read(PATCH_ADDRESS);
  _patch = _patch >= MIN_PATCH && _patch <= MAX_PATCH ? _patch : MIN_PATCH;
}

void Store::saveBank(byte bank)
{
  _bank = bank;
  EEPROM.write(BANK_ADDRESS, bank);
}

void Store::savePatch(byte patch)
{
  _patch = patch;
  EEPROM.write(PATCH_ADDRESS, patch);
}

byte Store::getBank() const
{
  return _bank;
}

byte Store::getPatch() const
{
  return _patch;
}
