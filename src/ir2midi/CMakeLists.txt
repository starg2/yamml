
set(IR2MIDIHeaders
    ../../include/ir2midi/command.hpp
    ../../include/ir2midi/context.hpp
    ../../include/ir2midi/ir2midi.hpp
    command_program.hpp
    command_tempo.hpp
)

set(IR2MIDISources
    command_program.cpp
    command_tempo.cpp
    context.cpp
    ir2midi.cpp
)

yamml_add_library(IR2MIDI STATIC IR2MIDISources IR2MIDIHeaders)
