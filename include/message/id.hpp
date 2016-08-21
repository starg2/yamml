
#pragma once

namespace YAMML
{

namespace Message
{

enum class MessageID : int
{
    Unknown,
    UnknownInPhrase2IR,
    UnknownInComposition2IR,
    UnknownInIR2MIDI,
    UnprocessedAttribute,
    UnprocessedCommand,
    DuplicatedCompositionName,
    DuplicatedPhraseName,
    NoSuchPhraseName,
    NoSuchCompositionName,
    TrackNumberIsOutOfSafeRange,
    TrackNumberIsOutOfPreferredRange,
    TooLargeRepeatCount,

    // error_attribute.hpp
    GrammarAttributeArgument,
    GrammarCommaSeparatedOptionalAttributeArguments,
    GrammarAttributeList,
    GrammarAttributeNameAndArguments,
    GrammarEndBracket,

    // error_common.hpp
    GrammarBlockBegin,
    GrammarBlockEnd,
    GrammarEndParenthesis,

    // error_composition.hpp
    GrammarCommandArgument,
    GrammarCommandsAndTrackListBlocks,
    GrammarOneOrMoreTrackBlocks,
    GrammarSpaceSeparatedOptionalTrackItems,

    // error_literal.hpp
    GrammarSeparators,
    GrammarSeparator,
    GrammarEOF,
    GrammarUntilBlockCommentEnd,
    GrammarIdentifier,
    GrammarEscape,
    GrammarAnyChar,
    GrammarNoDigit,
    GrammarXDigit,
    GrammarUntilDoubleQuote,
    GrammarUntilSingleQuote,

    // error_module.hpp
    GrammarPhrasesAndCompositions,

    // grammar_phrase.hpp
    GrammarNoteSequenceBlockWithoutAttributes,
    GrammarSimpleDurationModifier,
    GrammarNoteNumber,
    GrammarNoteSequenceStatementsAndBlocks,
    GrammarNoteSequenceAndSeparators,
    GrammarSpaceDelimitedChords,
    GrammarDurationOrDurationSet,
    GrammarSpaceDelimitedDurations,
    GrammarPhrase2,
    GrammarPhrase3,
    GrammarPhrase4,
    GrammarPhraseName
};

} // namespace Message

} // namespace YAMML
