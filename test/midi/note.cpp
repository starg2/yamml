
#include <boost/test/unit_test.hpp>

#include <midi/note.hpp>

using namespace YAMML::MIDI;

BOOST_AUTO_TEST_SUITE(NoteTest)

BOOST_AUTO_TEST_CASE(NoteName1)
{
    BOOST_CHECK_EQUAL((NoteNumber{NoteName::AFlat, 0}.GetMIDINoteNumber()), 8);
    BOOST_CHECK_EQUAL((NoteNumber{NoteName::E, 5}.GetMIDINoteNumber()), 64);
    BOOST_CHECK_EQUAL((NoteNumber{NoteName::DSharp, 9}.GetMIDINoteNumber()), 111);
}

BOOST_AUTO_TEST_SUITE_END()
