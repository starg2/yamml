
#include <boost/test/unit_test.hpp>

#include <pegtl.hh>
#include <pegtl/trace.hh>

#include "../../src/parser/parser_composition.hpp"

using namespace YAMML::Parser::Grammar;

BOOST_AUTO_TEST_SUITE(CompositionParserTest)

BOOST_AUTO_TEST_CASE(CompositionTest1)
{
    std::string data = R"(

composition Foo
{
    tracks
    {
        [program(0)] 1
        {
            Bar;
        }
    }
}

)";

    bool result = pegtl::parse<pegtl::must<Composition, Separators, pegtl::eof>>(data, "source");
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_SUITE_END()
