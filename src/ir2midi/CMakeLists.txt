
set(IR2MIDIHeaders
    ../../include/ir2midi/command.hpp
    ../../include/ir2midi/context.hpp
    ../../include/ir2midi/ir2midi.hpp
    command_insert.hpp
    command_panpot.hpp
    command_program.hpp
    command_tempo.hpp
    command_volume.hpp
)

set(IR2MIDISources
    command.cpp
    command_insert.cpp
    command_panpot.cpp
    command_program.cpp
    command_tempo.cpp
    command_volume.cpp
    context.cpp
    ir2midi.cpp
)

yamml_add_library(IR2MIDI STATIC IR2MIDISources IR2MIDIHeaders)
