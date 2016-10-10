
#include <boost/test/unit_test.hpp>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4244) // conversion from 'const std::streamsize' to 'std::size_t', possible loss of data
#endif

#include <pegtl.hh>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include "../../src/parser/parser_phrase.hpp"

using namespace YAMML::Parser::Grammar;

BOOST_AUTO_TEST_SUITE(PhraseParserTest)

BOOST_AUTO_TEST_CASE(NoteNameTest1)
{
    std::string data = "As";
    bool result = pegtl::parse_string<pegtl::must<NoteName, Separators, pegtl::eof>>(data, "source");
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(NoteNumberTest1)
{
    std::string data = "Bf4";
    bool result = pegtl::parse_string<pegtl::must<NoteNumber, Separators, pegtl::eof>>(data, "source");
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(NoteAndDurationTest1)
{
    std::string data = "Bf4/8";
    bool result = pegtl::parse_string<pegtl::must<NoteAndDuration, Separators, pegtl::eof>>(data, "source");
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(NoteAndDurationTest2)
{
    std::string data = "Bf4/*/*/ / 8";
    bool result = pegtl::parse_string<pegtl::must<NoteAndDuration, Separators, pegtl::eof>>(data, "source");
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(PhraseTest1)
{
    std::string data = R"(

phrase Foo
{
    CDEF GABR;
    2%(4%4*(C/8 D/8) E/8F/8);
}

)";

    bool result = pegtl::parse_string<pegtl::must<Phrase, Separators, pegtl::eof>>(data, "source");
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(PhraseTest2)
{
    std::string data = R"(

phrase Bar
{
    [transform(velocity, set, 100), foo]
    {
        4%(C/8 D/8) & E/2;
    }
}

)";

    bool result = pegtl::parse_string<pegtl::must<Phrase, Separators, pegtl::eof>>(data, "source");
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_SUITE_END()
