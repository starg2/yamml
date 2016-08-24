
#pragma once

#include <message/id.hpp>

#include "error_control.hpp"
#include "parser_phrase.hpp"

namespace YAMML
{

namespace Parser
{

template<>
const Message::MessageID ErrorControl<Grammar::NoteSequenceBlockWithoutAttributes>::ID = Message::MessageID::GrammarNoteSequenceBlockWithoutAttributes;

template<>
const Message::MessageID ErrorControl<Grammar::NoteNumber>::ID = Message::MessageID::GrammarNoteNumber;

template<>
const Message::MessageID ErrorControl<Grammar::NoteSequenceStatementsAndBlocks>::ID = Message::MessageID::GrammarNoteSequenceStatementsAndBlocks;

template<>
const Message::MessageID ErrorControl<Grammar::NoteSequenceAndSeparators>::ID = Message::MessageID::GrammarNoteSequenceAndSeparators;

template<>
const Message::MessageID ErrorControl<Grammar::SpaceDelimitedChords>::ID = Message::MessageID::GrammarSpaceDelimitedChords;

template<>
const Message::MessageID ErrorControl<Grammar::DurationOrDurationSet>::ID = Message::MessageID::GrammarDurationOrDurationSet;

template<>
const Message::MessageID ErrorControl<Grammar::SpaceDelimitedDurations>::ID = Message::MessageID::GrammarSpaceDelimitedDurations;

template<>
const Message::MessageID ErrorControl<Grammar::TrapMissingRepeatOperator>::ID = Message::MessageID::GrammarMissingRepeatOperator;

template<>
const Message::MessageID ErrorControl<
    pegtl::sor<Grammar::NoteSequenceInParentheses, Grammar::NoteAndDuration>
>::ID = Message::MessageID::GrammarPhrase2;

template<>
const Message::MessageID ErrorControl<
    pegtl::sor<Grammar::NoteSequenceInParentheses, Grammar::NoteRepeatExpression, Grammar::NoteAndDuration>
>::ID = Message::MessageID::GrammarPhrase3;

template<>
const Message::MessageID ErrorControl<
    pegtl::sor<Grammar::NoteSequenceInParentheses, Grammar::NoteRepeatEachExpression, Grammar::NoteRepeatExpression, Grammar::TrapMissingRepeatOperator, Grammar::NoteAndDuration>
>::ID = Message::MessageID::GrammarPhrase4;

template<>
const Message::MessageID ErrorControl<Grammar::PhraseName>::ID = Message::MessageID::GrammarPhraseName;

} // namespace Parser

} // namespace YAMML
