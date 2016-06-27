
#include <boost/test/unit_test.hpp>

#include <midi/file.hpp>

using namespace YAMML::MIDI;

BOOST_AUTO_TEST_SUITE(FileFormatTest)

BOOST_AUTO_TEST_CASE(Header1)
{
    MIDIFile f;
    f.Header.SetFormat(1);
    BOOST_CHECK_EQUAL(f.Header.GetFormat(), 1);
}

BOOST_AUTO_TEST_SUITE_END()
