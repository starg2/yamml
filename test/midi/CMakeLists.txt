
set(MIDITestHeaders
    ../../include/midi/event.hpp
    ../../include/midi/file.hpp
    ../../include/midi/header.hpp
    ../../include/midi/note.hpp
    ../../include/midi/track.hpp
    )

yamml_add_test_one(FileFormatTest fileformattest.cpp MIDITestHeaders)
yamml_add_test_one(NoteTest note.cpp MIDITestHeaders)
