
#include "MidiManager.h"

MidiManager::MidiManager() :
  _midi(midi::MidiInterface<HardwareSerial>(Serial))
{
  _midi.begin(MIDI_CHANNEL_OFF);
}
