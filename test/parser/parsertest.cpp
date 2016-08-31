
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
        auto itPhrase = module->Phrases.begin();

        BOOST_REQUIRE(itPhrase != module->Phrases.end());

        auto phrase = *itPhrase;

        BOOST_CHECK_EQUAL(phrase.Name, "Foo");
        BOOST_CHECK(phrase.Attributes.empty());
        BOOST_CHECK_EQUAL(phrase.Location.Line, 3u);
        BOOST_CHECK_EQUAL(phrase.Location.Column, 0u);

        BOOST_CHECK_EQUAL(phrase.Block.Location.Line, 4u);
        BOOST_CHECK_EQUAL(phrase.Block.Location.Column, 0u);

        {
            auto noteSeqStatement = boost::get<YAMML::AST::NoteSequenceStatement>(phrase.Block.Sequences.at(0));
            BOOST_CHECK(noteSeqStatement.Attributes.empty());

            BOOST_REQUIRE(noteSeqStatement.NoteSeq.is_initialized());
            auto noteSeq = *noteSeqStatement.NoteSeq;

            BOOST_REQUIRE_EQUAL(noteSeq.Notes.size(), 1u);

            auto noteAndExpr = boost::get<AST::NoteAndExpression>(noteSeq.Notes.at(0));
            BOOST_REQUIRE_EQUAL(noteAndExpr.Notes.size(), 3u);

            {
                auto noteAndDuration = boost::get<AST::NoteAndDuration>(noteAndExpr.Notes.at(0).get().Notes.at(0));
                BOOST_CHECK(!noteAndDuration.Duration.is_initialized());
                BOOST_CHECK_EQUAL(boost::get<AST::NoteNumber>(noteAndDuration.Note).Name.Name, 'C');
                BOOST_CHECK(!boost::get<AST::NoteNumber>(noteAndDuration.Note).Octave.is_initialized());
            }

            {
                auto noteAndDuration = boost::get<AST::NoteAndDuration>(noteAndExpr.Notes.at(1).get().Notes.at(0));
                BOOST_CHECK(!noteAndDuration.Duration.is_initialized());
                BOOST_CHECK_EQUAL(boost::get<AST::NoteNumber>(noteAndDuration.Note).Name.Name, 'E');
                BOOST_CHECK(!boost::get<AST::NoteNumber>(noteAndDuration.Note).Octave.is_initialized());
            }

            {
                auto noteAndDuration = boost::get<AST::NoteAndDuration>(noteAndExpr.Notes.at(2).get().Notes.at(0));
                BOOST_CHECK(!noteAndDuration.Duration.is_initialized());
                BOOST_CHECK_EQUAL(boost::get<AST::NoteNumber>(noteAndDuration.Note).Name.Name, 'G');
                BOOST_CHECK(!boost::get<AST::NoteNumber>(noteAndDuration.Note).Octave.is_initialized());
            }
        }
    }

    {
        auto itComposition = module->Compositions.begin();
        BOOST_REQUIRE(itComposition != module->Compositions.end());

        auto composition = *itComposition;

        BOOST_CHECK_EQUAL(composition.Name, "Main");
        BOOST_CHECK(composition.Attributes.empty());
        BOOST_CHECK_EQUAL(composition.Location.Line, 8u);
        BOOST_CHECK_EQUAL(composition.Location.Column, 0u);

        BOOST_CHECK_EQUAL(composition.Statements.size(), 1u);

        auto trackListBlock = boost::get<AST::TrackListBlock>(composition.Statements.at(0));
        BOOST_CHECK_EQUAL(trackListBlock.Tracks.size(), 1u);

        auto trackBlock = trackListBlock.Tracks.at(0);
        BOOST_CHECK_EQUAL(trackBlock.TrackNumber, 1);
        BOOST_CHECK(trackBlock.Attributes.empty());
        BOOST_CHECK_EQUAL(trackBlock.Location.Line, 12u);
        BOOST_CHECK_EQUAL(trackBlock.Location.Column, 8u);
        BOOST_CHECK_EQUAL(trackBlock.Items.size(), 1u);

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

    auto itComposition = module->Compositions.begin();
    BOOST_REQUIRE(itComposition != module->Compositions.end());

    BOOST_CHECK_EQUAL(itComposition->Name, "Main");
    BOOST_CHECK_EQUAL(itComposition->Attributes.size(), 1u);
    
    auto attr = itComposition->Attributes.at(0);

    BOOST_CHECK_EQUAL(attr.Name, "foo");
    BOOST_CHECK_EQUAL(attr.Location.Line, 3u);
    BOOST_CHECK_EQUAL(attr.Location.Column, 0u);

    BOOST_CHECK_EQUAL(attr.Arguments.size(), 1u);

    auto arg = attr.Arguments.at(0);

    BOOST_REQUIRE(arg.Name.is_initialized());
    BOOST_CHECK_EQUAL(arg.Name.value(), "bar");

    auto argValue = arg.Value.Value;

    BOOST_CHECK_EQUAL(boost::get<std::string>(argValue), u8"abc\u2661");

    BOOST_CHECK_EQUAL(arg.Location.Line, 3u);
    BOOST_CHECK_EQUAL(arg.Location.Column, 5u);

    BOOST_CHECK_EQUAL(arg.Value.Location.Line, 3u);
    BOOST_CHECK_EQUAL(arg.Value.Location.Column, 9u);
}

BOOST_AUTO_TEST_SUITE_END()
