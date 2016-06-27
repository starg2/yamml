
#include <cstdint>

#include <vector>

#include <boost/test/unit_test.hpp>

#include <binary/binarywriter.hpp>

using namespace YAMML::Binary;

BOOST_AUTO_TEST_SUITE(BinaryWriterTest)

BOOST_AUTO_TEST_CASE(SimpleTest)
{
    BinaryWriter writer;

    writer.AppendUInt8(2);
    writer.AppendUInt8(3);

    auto buf = writer.GetBuffer();
    std::vector<std::uint8_t> expected{2, 3};

    BOOST_CHECK_EQUAL_COLLECTIONS(buf.begin(), buf.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(ComplexTest)
{
    BinaryWriter writer;

    writer.AppendUInt16BE(0x0203);
    writer.AppendUInt32BE(0x204060);

    auto buf = writer.GetBuffer();
    std::vector<std::uint8_t> expected{0x02, 0x03, 0x00, 0x20, 0x40, 0x60};

    BOOST_CHECK_EQUAL_COLLECTIONS(buf.begin(), buf.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(StringTest)
{
    BinaryWriter writer;

    writer.AppendString("ABC");

    auto buf = writer.GetBuffer();
    std::vector<std::uint8_t> expected{'A', 'B', 'C'};

    BOOST_CHECK_EQUAL_COLLECTIONS(buf.begin(), buf.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(BufferTest)
{
    BinaryWriter writer;

    writer.AppendUInt16BE(0x0203);
    writer.AppendUInt8(3);

    BinaryWriter writer2;
    writer2.AppendUInt16BE(0x4060);
    
    auto buf = writer.GetBuffer();
    writer2.AppendBuffer(buf.begin(), buf.end());

    auto buf2 = writer2.GetBuffer();
    std::vector<std::uint8_t> expected{0x02, 0x03, 3};
    std::vector<std::uint8_t> expected2{0x40, 0x60, 0x02, 0x03, 3};

    BOOST_CHECK_EQUAL_COLLECTIONS(buf.begin(), buf.end(), expected.begin(), expected.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(buf2.begin(), buf2.end(), expected2.begin(), expected2.end());
}

BOOST_AUTO_TEST_SUITE_END()
