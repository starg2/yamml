
#include <stdexcept>
#include <string>
#include <vector>

#include "format.hpp"
#include "msgcallback.hpp"


namespace YAMML
{

namespace Driver
{

namespace
{

const std::string ICEMessage =
    "An internal compiler error has occurred during the compilation process. "
    "Please submit a bug report with full source code if appropriate.";

}

MessagePrinter::MessagePrinter(IStdErrWriter* pStdErrWriter)
    : m_KindTextMap{
        {Message::MessageKind::FetalError, "fetal error"},
        {Message::MessageKind::Error, "error"},
        {Message::MessageKind::Warning, "warning"},
        {Message::MessageKind::Note, "note"}
    },
    m_IDTextMap{
        {Message::MessageID::Unknown, ICEMessage},
        {Message::MessageID::UnknownInPhrase2IR, ICEMessage + " (Phrase2IR: Phrase = '{0}', Message = '{1}')"},
        {Message::MessageID::UnknownInComposition2IR, ICEMessage + " (Composition2IR: Composition = '{0}', Message = '{1}')"},
        {Message::MessageID::UnknownInIR2MIDI, ICEMessage + " (IR2MIDI: Message = '{0}')"},
        {Message::MessageID::UnprocessedCommand, ICEMessage + " (Unprocessed command in '{0}')"},
        {Message::MessageID::UnprocessedAttribute, ICEMessage + " (Unprocessed attribute in '{0}')"},
        {Message::MessageID::DuplicatedCompositionName, "composition named '{0}' is already defined."},
        {Message::MessageID::DuplicatedPhraseName, "phrase named '{0}' is already defined."},
        {Message::MessageID::NoSuchPhraseName, "cannot find a phrase named '{0}'."},
        {Message::MessageID::NoSuchCompositionName, "cannot find a composition named '{0}'."},
        {Message::MessageID::TrackNumberIsOutOfSafeRange, "track number '{0}' is out of range. (must be 0 <= # < {1})"},
        {Message::MessageID::TrackNumberIsOutOfPreferredRange, "track number '{0}' is out of range. (must be 0 <= # < {1})"},
        {Message::MessageID::TooLargeRepeatCount, "repeat count '{0}' is too large. (must be <= {1})"},

        // error_attribute.hpp
        {Message::MessageID::GrammarAttributeArgument, "parse error: GrammarAttributeArgument"},
        {Message::MessageID::GrammarCommaSeparatedOptionalAttributeArguments, "parse error: GrammarCommaSeparatedOptionalAttributeArguments"},
        {Message::MessageID::GrammarAttributeList, "parse error: GrammarAttributeList"},
        {Message::MessageID::GrammarAttributeNameAndArguments, "parse error: GrammarAttributeNameAndArguments"},
        {Message::MessageID::GrammarEndBracket, "parse error: GrammarEndBracket"},

        // error_common.hpp
        {Message::MessageID::GrammarBlockBegin, "parse error: GrammarBlockBegin"},
        {Message::MessageID::GrammarBlockEnd, "parse error: GrammarBlockEnd"},
        {Message::MessageID::GrammarEndParenthesis, "parse error: GrammarEndParenthesis"},

        // error_composition.hpp
        {Message::MessageID::GrammarCommandArgument, "parse error: GrammarCommandArgument"},
        {Message::MessageID::GrammarCommandsAndTrackListBlocks, "parse error: GrammarCommandsAndTrackListBlocks"},
        {Message::MessageID::GrammarOneOrMoreTrackBlocks, "parse error: GrammarOneOrMoreTrackBlocks"},
        {Message::MessageID::GrammarSpaceSeparatedOptionalTrackItems, "parse error: GrammarSpaceSeparatedOptionalTrackItems"},

        // error_literal.hpp
        {Message::MessageID::GrammarSeparators, "parse error: GrammarSeparators"},
        {Message::MessageID::GrammarSeparator, "parse error: GrammarSeparator"},
        {Message::MessageID::GrammarEOF, "parse error: GrammarEOF"},
        {Message::MessageID::GrammarUntilBlockCommentEnd, "parse error: GrammarUntilBlockCommentEnd"},
        {Message::MessageID::GrammarIdentifier, "parse error: GrammarIdentifier"},
        {Message::MessageID::GrammarEscape, "parse error: GrammarEscape"},
        {Message::MessageID::GrammarAnyChar, "parse error: GrammarAnyChar"},
        {Message::MessageID::GrammarNoDigit, "parse error: GrammarNoDigit"},
        {Message::MessageID::GrammarXDigit, "parse error: GrammarXDigit"},
        {Message::MessageID::GrammarUntilDoubleQuote, "parse error: GrammarUntilDoubleQuote"},
        {Message::MessageID::GrammarUntilSingleQuote, "parse error: GrammarUntilSingleQuote"},

        // error_module.hpp
        {Message::MessageID::GrammarPhrasesAndCompositions, "parse error: GrammarPhrasesAndCompositions"},

        // grammar_phrase.hpp
        {Message::MessageID::GrammarNoteSequenceBlockWithoutAttributes, "parse error: GrammarNoteSequenceBlockWithoutAttributes"},
        {Message::MessageID::GrammarSimpleDurationModifier, "parse error: GrammarSimpleDurationModifier"},
        {Message::MessageID::GrammarNoteNumber, "parse error: GrammarNoteNumber"},
        {Message::MessageID::GrammarNoteSequenceStatementsAndBlocks, "parse error: GrammarNoteSequenceStatementsAndBlocks"},
        {Message::MessageID::GrammarNoteSequenceAndSeparators, "parse error: GrammarNoteSequenceAndSeparators"},
        {Message::MessageID::GrammarSpaceDelimitedChords, "parse error: GrammarSpaceDelimitedChords"},
        {Message::MessageID::GrammarDurationOrDurationSet, "parse error: GrammarDurationOrDurationSet"},
        {Message::MessageID::GrammarSpaceDelimitedDurations, "parse error: GrammarSpaceDelimitedDurations"},
        {Message::MessageID::GrammarMissingRepeatOperator, "expecting '*' or '%' after a numeric constant."},
        {Message::MessageID::GrammarPhrase2, "parse error: GrammarPhrase2"},
        {Message::MessageID::GrammarPhrase3, "parse error: GrammarPhrase3"},
        {Message::MessageID::GrammarPhrase4, "parse error: GrammarPhrase4"},
        {Message::MessageID::GrammarPhraseName, "parse error: GrammarPhraseName"}
    },
    m_pStdErrWriter{pStdErrWriter}
{
    if (!pStdErrWriter)
    {
        throw std::invalid_argument("MessagePrinter::MessagePrinter(pStdErrWriter): pStdErrWriter is null");
    }
}

bool MessagePrinter::operator()(const Message::MessageItem& item) const
{
    std::string output = item.SourceName;

    if (item.Location.Line != 0)
    {
        output += '(' + std::to_string(item.Location.Line) + ')';
    }

    output += ": " + m_KindTextMap.at(item.Kind) + ": " + FormatText(m_IDTextMap.at(item.ID), item.Arguments);

    m_pStdErrWriter->WriteLine(output);
    return true;
}

} // namespace Driver

} // namespace YAMML
