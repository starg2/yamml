
set(IR2MIDIHeaders
    ../../include/ir2midi/ir2midi.hpp
)

set(IR2MIDISources
    ir2midi.cpp
)

yamml_add_library(IR2MIDI STATIC IR2MIDISources IR2MIDIHeaders)
