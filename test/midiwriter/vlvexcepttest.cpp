
#include <cstdint>

#include <vector>

#include <boost/test/unit_test.hpp>

#include <exceptions/invalidarg.hpp>
#include <midiwriter/variablelengthvalue.hpp>

using namespace YAMML::MIDIWriter;

BOOST_AUTO_TEST_SUITE(VLVExceptTest)

BOOST_AUTO_TEST_CASE(Test1)
{
    BOOST_CHECK_THROW(ToVariableLengthValue(0x80FFFFFF), YAMML::Exceptions::InvalidArgumentException);
}

BOOST_AUTO_TEST_SUITE_END()
