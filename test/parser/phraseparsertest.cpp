
#include <boost/test/unit_test.hpp>

#include <pegtl.hh>
#include <pegtl/trace.hh>

#include "../../src/parser/parser_phrase.hpp"

using namespace YAMML::Parser::Grammar;

BOOST_AUTO_TEST_SUITE(PhraseParserTest)

BOOST_AUTO_TEST_CASE(NoteNameTest1)
{
    std::string data = "As";
    bool result = pegtl::parse<pegtl::must<NoteName, Separators, pegtl::eof>>(data, "source");
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(NoteNumberTest1)
{
    std::string data = "Bf4";
    bool result = pegtl::parse<pegtl::must<NoteNumber, Separators, pegtl::eof>>(data, "source");
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(NoteAndDurationTest1)
{
    std::string data = "Bf4/8";
    bool result = pegtl::parse<pegtl::must<NoteAndDuration, Separators, pegtl::eof>>(data, "source");
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(NoteAndDurationTest2)
{
    std::string data = "Bf4/*/*/ / 8";
    bool result = pegtl::parse<pegtl::must<NoteAndDuration, Separators, pegtl::eof>>(data, "source");
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(PhraseTest1)
{
    std::string data = R"(

phrase Foo
{
    CDEF GABR;
    2%(C/8D/8 E/8F/8);
}

)";

    bool result = pegtl::parse<pegtl::must<Phrase, Separators, pegtl::eof>>(data, "source");
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_SUITE_END()
