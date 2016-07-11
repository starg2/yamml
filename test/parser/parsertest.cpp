
#include <string>

#include <boost/test/unit_test.hpp>

#include <parser/parser.hpp>

using namespace YAMML::Parser;

BOOST_AUTO_TEST_SUITE(ParserTest)

BOOST_AUTO_TEST_CASE(ParserTest1)
{
    std::string source = R"(

phrase Foo
{
    C&E&G;
}

composition Main
{
    tracks
    {
        1
        {
            Foo;
        }
    }
}

)";

    YAMMLParser parser("test.ym1", source);

    bool result = parser.Parse();
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(ParserTest2)
{
    std::string source = R"(

composition Main
{
    tracks
    {
        {
            Foo;
        }
    }
}

)";

    YAMMLParser parser("test.ym1", source);

    bool result = parser.Parse();
    BOOST_CHECK(!result);
}

BOOST_AUTO_TEST_SUITE_END()
