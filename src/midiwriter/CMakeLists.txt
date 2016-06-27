
set(MIDIWriterHeaders
    ../../include/binary/binarywriter.hpp
    ../../include/midiwriter/midiwriter.hpp
    ../../include/midiwriter/variablelengthvalue.hpp
    trackeventwriter.hpp
)

set(MIDIWriterSources
    midiwriter.cpp
    variablelengthvalue.cpp
)

yamml_add_library(MIDIWriter STATIC MIDIWriterSources MIDIWriterHeaders)
