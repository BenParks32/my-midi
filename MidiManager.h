#ifndef _midi_manager_h
#define _midi_manager_h

#include <MIDI.h>

class MidiManager 
{
  public:
    MidiManager();
    
  private:
    midi::MidiInterface<HardwareSerial> _midi;
};

#endif
