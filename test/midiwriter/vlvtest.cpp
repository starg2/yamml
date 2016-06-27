
#include <cstdint>

#include <vector>

#include <boost/test/unit_test.hpp>

#include <midiwriter/variablelengthvalue.hpp>

using namespace YAMML::MIDIWriter;

BOOST_AUTO_TEST_SUITE(VLVTest)

BOOST_AUTO_TEST_CASE(Test1)
{
    std::vector<std::uint8_t> expected{0x7F};
    auto ret = ToVariableLengthValue(0x7F);

    BOOST_CHECK_EQUAL_COLLECTIONS(ret.begin(), ret.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(Test2)
{
    std::vector<std::uint8_t> expected{0x81, 0x7F};
    auto ret = ToVariableLengthValue(0xFF);

    BOOST_CHECK_EQUAL_COLLECTIONS(ret.begin(), ret.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(Test3)
{
    std::vector<std::uint8_t> expected{0x82, 0x80, 0x00};
    auto ret = ToVariableLengthValue(0x8000);

    BOOST_CHECK_EQUAL_COLLECTIONS(ret.begin(), ret.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_SUITE_END()
