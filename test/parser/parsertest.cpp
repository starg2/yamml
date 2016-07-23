
#include <string>

#include <boost/test/unit_test.hpp>

#include <ast/module.hpp>
#include <parser/parser.hpp>

using namespace YAMML;
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
    BOOST_REQUIRE(result);

    auto module = parser.GetAST();

    {
        auto itPhrase = module->Phrases.find("Foo");

        BOOST_REQUIRE(itPhrase != module->Phrases.end());

        BOOST_CHECK_EQUAL(itPhrase->first, "Foo");

        auto phrase = itPhrase->second;

        BOOST_CHECK_EQUAL(phrase.Name, "Foo");
        BOOST_CHECK(phrase.Attributes.empty());
        BOOST_CHECK_EQUAL(phrase.Location.Line, 3);
        BOOST_CHECK_EQUAL(phrase.Location.Column, 0);

        BOOST_CHECK_EQUAL(phrase.Block.Location.Line, 4);
        BOOST_CHECK_EQUAL(phrase.Block.Location.Column, 0);

        {
            auto noteSeqStatement = boost::get<YAMML::AST::NoteSequenceStatement>(phrase.Block.Sequences.at(0));
            BOOST_CHECK(noteSeqStatement.Attributes.empty());

            BOOST_REQUIRE(noteSeqStatement.NoteSeq.is_initialized());
            auto noteSeq = *noteSeqStatement.NoteSeq;

            BOOST_CHECK_EQUAL(noteSeq.Notes.size(), 1);

            auto noteAndExpr = noteSeq.Notes.at(0);
            BOOST_CHECK_EQUAL(noteAndExpr.Notes.size(), 3);

            {
                auto noteAndDuration = boost::get<AST::NoteAndDuration>(noteAndExpr.Notes.at(0));
                BOOST_CHECK(!noteAndDuration.Duration.is_initialized());
                BOOST_CHECK_EQUAL(boost::get<AST::NoteNumber>(noteAndDuration.Note).Name.Name, 'C');
                BOOST_CHECK(!boost::get<AST::NoteNumber>(noteAndDuration.Note).Octave.is_initialized());
            }

            {
                auto noteAndDuration = boost::get<AST::NoteAndDuration>(noteAndExpr.Notes.at(1));
                BOOST_CHECK(!noteAndDuration.Duration.is_initialized());
                BOOST_CHECK_EQUAL(boost::get<AST::NoteNumber>(noteAndDuration.Note).Name.Name, 'E');
                BOOST_CHECK(!boost::get<AST::NoteNumber>(noteAndDuration.Note).Octave.is_initialized());
            }

            {
                auto noteAndDuration = boost::get<AST::NoteAndDuration>(noteAndExpr.Notes.at(2));
                BOOST_CHECK(!noteAndDuration.Duration.is_initialized());
                BOOST_CHECK_EQUAL(boost::get<AST::NoteNumber>(noteAndDuration.Note).Name.Name, 'G');
                BOOST_CHECK(!boost::get<AST::NoteNumber>(noteAndDuration.Note).Octave.is_initialized());
            }
        }
    }

    {
        auto itComposition = module->Compositions.find("Main");
        BOOST_REQUIRE(itComposition != module->Compositions.end());

        BOOST_CHECK_EQUAL(itComposition->first, "Main");

        auto composition = itComposition->second;

        BOOST_CHECK_EQUAL(composition.Name, "Main");
        BOOST_CHECK(composition.Attributes.empty());
        BOOST_CHECK_EQUAL(composition.Location.Line, 8);
        BOOST_CHECK_EQUAL(composition.Location.Column, 0);

        BOOST_CHECK_EQUAL(composition.Statements.size(), 1);

        auto trackListBlock = boost::get<AST::TrackListBlock>(composition.Statements.at(0));
        BOOST_CHECK_EQUAL(trackListBlock.Tracks.size(), 1);

        auto trackBlock = trackListBlock.Tracks.at(0);
        BOOST_CHECK_EQUAL(trackBlock.TrackNumber, 1);
        BOOST_CHECK(trackBlock.Attributes.empty());
        BOOST_CHECK_EQUAL(trackBlock.Location.Line, 12);
        BOOST_CHECK_EQUAL(trackBlock.Location.Column, 8);
        BOOST_CHECK_EQUAL(trackBlock.Items.size(), 1);

        auto trackItem = trackBlock.Items.at(0);

        BOOST_CHECK_EQUAL(trackItem.PhraseName, "Foo");
        BOOST_CHECK(trackItem.Attributes.empty());
    }
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

BOOST_AUTO_TEST_CASE(ParserTest3)
{
    std::string source = R"(

[foo(bar="abc\u2661")]
composition Main
{
}

)";

    YAMMLParser parser("test.ym1", source);

    bool result = parser.Parse();
    BOOST_REQUIRE(result);

    auto module = parser.GetAST();
    BOOST_REQUIRE(module.is_initialized());

    auto itComposition = module->Compositions.find("Main");
    BOOST_REQUIRE(itComposition != module->Compositions.end());

    BOOST_CHECK_EQUAL(itComposition->first, "Main");
    BOOST_CHECK_EQUAL(itComposition->second.Attributes.size(), 1);
    
    auto attr = itComposition->second.Attributes.at(0);

    BOOST_CHECK_EQUAL(attr.Name, "foo");
    BOOST_CHECK_EQUAL(attr.Location.Line, 3);
    BOOST_CHECK_EQUAL(attr.Location.Column, 0);

    BOOST_CHECK_EQUAL(attr.Arguments.size(), 1);

    auto arg = attr.Arguments.at(0);

    BOOST_REQUIRE(arg.Name.is_initialized());
    BOOST_CHECK_EQUAL(arg.Name.value(), "bar");

    auto argValue = arg.Value.Value;

    BOOST_CHECK_EQUAL(boost::get<std::string>(argValue), u8"abc\u2661");

    BOOST_CHECK_EQUAL(arg.Location.Line, 3);
    BOOST_CHECK_EQUAL(arg.Location.Column, 5);

    BOOST_CHECK_EQUAL(arg.Value.Location.Line, 3);
    BOOST_CHECK_EQUAL(arg.Value.Location.Column, 9);
}

BOOST_AUTO_TEST_SUITE_END()
