#ifndef _store_h
#define _store_h

const byte MIN_BANK = 0;
const byte MAX_BANK = 39;

const byte MIN_PATCH = 0;
const byte MAX_PATCH = 2;

class Store
{
  public:
    Store();

  private:
    Store(const Store& rhs);

  public:
    void save() const;
    void load();

    void saveBank(byte bank);
    void savePatch(byte patch);   

    byte getBank() const;
    byte getPatch() const;

  private:
     word _bank;
     word _patch;
};

#endif
