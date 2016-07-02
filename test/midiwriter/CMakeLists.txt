
set(MIDIWriterTestHeaders
    ../../include/midiwriter/midiwriter.hpp
    ../../include/midiwriter/variablelengthvalue.hpp
)

yamml_add_test_one(MIDIWriterTest midiwritertest.cpp MIDIWriterTestHeaders)
target_link_libraries(MIDIWriterTest_Exe MIDIWriter)

yamml_add_test_one(VLVTest vlvtest.cpp MIDIWriterTestHeaders)
target_link_libraries(VLVTest_Exe MIDIWriter)

yamml_add_test_one(VLVExceptTest vlvexcepttest.cpp MIDIWriterTestHeaders)
target_link_libraries(VLVExceptTest_Exe MIDIWriter)
