
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
        {Message::MessageID::UnprocessedAttribute, ICEMessage + " (Unprocessed attribute: '{0}')"},
        {Message::MessageID::DuplicatedCompositionName, "composition named '{0}' is already defined"},
        {Message::MessageID::DuplicatedPhraseName, "phrase named '{0}' is already defined"},
        {Message::MessageID::NoSuchPhraseName, "cannot find a phrase named '{0}'"},
        {Message::MessageID::NoSuchCompositionName, "cannot find a composition named '{0}'"},
        {Message::MessageID::TrackNumberIsOutOfSafeRange, "track number '{0}' is out of range (must be 0 <= # < {1})"},
        {Message::MessageID::TrackNumberIsOutOfPreferredRange, "track number '{0}' is out of range (must be 0 <= # < {1})"},
        {Message::MessageID::TooLargeRepeatCount, "repeat count '{0}' is too large (must be <= {1})"},
        {Message::MessageID::OctaveOutOfRange, "octave value '{0}' is out of range (must be between 0 and 10)"},

        // error_attribute.hpp
        {Message::MessageID::GrammarAttributeArgument, "expecting numeric constant, string literal or identifier after ','"},
        {Message::MessageID::GrammarCommaSeparatedOptionalAttributeArguments, "parse error: GrammarCommaSeparatedOptionalAttributeArguments"},
        {Message::MessageID::GrammarAttributeList, "expecting identifier after '['"},
        {Message::MessageID::GrammarAttributeNameAndArguments, "expecting identifier here (consider removing trailing ',')"},
        {Message::MessageID::GrammarEndBracket, "unexpected token '{0}'. Did you forget ']'?"},

        // error_common.hpp
        {Message::MessageID::GrammarBlockBegin, "unexpected token '{0}'. expecting '{'"},
        {Message::MessageID::GrammarBlockEnd, "unexpected token '{0}', did you forget ';' or '}'?"},
        {Message::MessageID::GrammarEndParenthesis, "unexpected token '{0}', did you forget ')'?"},
        {Message::MessageID::GrammarSemicolon, "unexpected token '{0}', did you forget ';'?"},

        // error_composition.hpp
        {Message::MessageID::GrammarCommandArgument, "expecting numeric constant, string literal or identifier after ','"},
        {Message::MessageID::GrammarCommandsAndTrackListBlocks, "parse error: GrammarCommandsAndTrackListBlocks"},
        {Message::MessageID::GrammarOneOrMoreTrackBlocks, "unexpected token '{0}'. expecting track blocks"},
        {Message::MessageID::GrammarSpaceSeparatedOptionalTrackItems, "parse error: GrammarSpaceSeparatedOptionalTrackItems"},
        {Message::MessageID::GrammarCompositionName, "expecting valid name for composition"},

        // error_literal.hpp
        {Message::MessageID::GrammarSeparators, "parse error: GrammarSeparators"},
        {Message::MessageID::GrammarSeparator, "parse error: GrammarSeparator"},
        {Message::MessageID::GrammarEOF, "expecting 'phrase' or 'composition'"},
        {Message::MessageID::GrammarUntilBlockCommentEnd, "unterminated block comment (missing '*/')"},
        {Message::MessageID::GrammarEscape, "invalid escape sequence"},
        {Message::MessageID::GrammarNoDigit, "numeric constant may not be prefixed with '0'"},
        {Message::MessageID::GrammarXDigit2, "expecting 2 xdigits after '\\x'"},
        {Message::MessageID::GrammarXDigit4, "expecting 4 xdigits after '\\u'"},
        {Message::MessageID::GrammarXDigit8, "expecting 8 xdigits after '\\U'"},
        {Message::MessageID::GrammarUntilDoubleQuote, "unterminated string literal (missing double quote)"},
        {Message::MessageID::GrammarUntilSingleQuote, "unterminated string literal (missing single quote)"},

        // error_module.hpp
        {Message::MessageID::GrammarPhrasesAndCompositions, "parse error: GrammarPhrasesAndCompositions"},

        // grammar_phrase.hpp
        {Message::MessageID::GrammarNoteSequenceBlockWithoutAttributes, "expecting '{' here"},
        {Message::MessageID::GrammarSimpleDurationModifier, "parse error: GrammarSimpleDurationModifier"},
        {Message::MessageID::GrammarNoteNumber, "parse error: GrammarNoteNumber"},
        {Message::MessageID::GrammarNoteSequenceStatementsAndBlocks, "parse error: GrammarNoteSequenceStatementsAndBlocks"},
        {Message::MessageID::GrammarNoteSequenceAndSeparators, "unexpected token '{0}', expecting notes"},
        {Message::MessageID::GrammarSpaceDelimitedChords, "parse error: GrammarSpaceDelimitedChords"},
        {Message::MessageID::GrammarDurationOrDurationSet, "expecting duration specifier after '/'"},
        {Message::MessageID::GrammarSpaceDelimitedDurations, "expecting duration specifiers, not '{0}'"},
        {Message::MessageID::GrammarMissingRepeatOperator, "expecting '*' or '%' after numeric constant"},
        {Message::MessageID::GrammarPhrase2, "unexpected token '{0}', expecting notes"},
        {Message::MessageID::GrammarPhrase3, "unexpected token '{0}', expecting notes"},
        {Message::MessageID::GrammarPhrase4, "unexpected token '{0}', expecting notes"},
        {Message::MessageID::GrammarPhraseName, "expecting valid name for phrase"},

        // IR2MIDI
        {Message::MessageID::InvalidCommandName, "invalid command '{0}'"},
        {Message::MessageID::WrongNumberOfCommandArguments, "wrong number of arguments passed to command '{0}'; {2} expected, {1} found"},
        {Message::MessageID::WrongTypeOfCommandArgument, "command argument {1} has a wrong type; expecting '{2}' here"},

        {Message::MessageID::InvalidProgram, "invalid program name"},
        {Message::MessageID::InvalidTempo, "invalid tempo value '{0}'"},
        {Message::MessageID::InvalidVolume, "invalid volume value '{0}'"}
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
