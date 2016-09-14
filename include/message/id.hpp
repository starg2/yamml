
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
    UnknownInIRProcessor,
    UnknownInIR2MIDI,
    UnprocessedAttribute,
    DuplicatedName,
    DuplicatedCompositionName,
    DuplicatedPhraseName,
    NoSuchPhraseName,
    NoSuchCompositionName,
    TrackNumberIsOutOfSafeRange,
    TrackNumberIsOutOfPreferredRange,
    TooLargeRepeatCount,
    IntegerOutOfRange,
    OctaveOutOfRange,

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
    GrammarSemicolon,

    // error_composition.hpp
    GrammarCommandArgument,
    GrammarCommandsAndTrackListBlocks,
    GrammarOneOrMoreTrackBlocks,
    GrammarSpaceSeparatedOptionalTrackItems,
    GrammarCompositionName,

    // error_literal.hpp
    GrammarSeparators,
    GrammarSeparator,
    GrammarEOF,
    GrammarUntilBlockCommentEnd,
    GrammarEscape,
    GrammarNoDigit,
    GrammarXDigit2,
    GrammarXDigit4,
    GrammarXDigit8,
    GrammarUntilDoubleQuote,
    GrammarUntilSingleQuote,

    // error_module.hpp
    GrammarTrapNeitherCompositionNorPhrase,
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
    GrammarMissingRepeatOperator,
    GrammarPhrase2,
    GrammarPhrase3,
    GrammarPhrase4,
    GrammarPhraseName,

    // IRProcessor
    InvalidAttributeName,

    // IR2MIDI
    InvalidCommandName,
    TrackBlockCompilationRecursion,
    TrackBlockCompilationBackTrace,
    TrackBlockCompilationBackTraceEntryPoint,
    WrongNumberOfCommandArguments,
    WrongTypeOfCommandArgument,

    InvalidPanpot,
    InvalidProgram,
    InvalidTempo,
    InvalidVolume
};

} // namespace Message

} // namespace YAMML
