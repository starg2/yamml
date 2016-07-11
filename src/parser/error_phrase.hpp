
#pragma once

#include <message/id.hpp>

#include "error_control.hpp"
#include "parser_phrase.hpp"

namespace YAMML
{

namespace Parser
{

template<>
const Message::MessageID ErrorControl<Grammar::NoteSequenceBlockWithoutAttributes>::ID = Message::MessageID::Unknown;

template<>
const Message::MessageID ErrorControl<Grammar::SimpleDurationModifier>::ID = Message::MessageID::Unknown;

template<>
const Message::MessageID ErrorControl<Grammar::NoteNumber>::ID = Message::MessageID::Unknown;

template<>
const Message::MessageID ErrorControl<Grammar::NoteSequenceStatementsAndBlocks>::ID = Message::MessageID::Unknown;

template<>
const Message::MessageID ErrorControl<Grammar::NoteSequenceAndSeparators>::ID = Message::MessageID::Unknown;

template<>
const Message::MessageID ErrorControl<Grammar::SpaceDelimitedChords>::ID = Message::MessageID::Unknown;

template<>
const Message::MessageID ErrorControl<Grammar::DurationOrDurationSet>::ID = Message::MessageID::Unknown;

template<>
const Message::MessageID ErrorControl<Grammar::SpaceDelimitedDurations>::ID = Message::MessageID::Unknown;

template<>
const Message::MessageID ErrorControl<
    pegtl::sor<Grammar::NoteSequenceInParentheses, Grammar::NoteAndDuration>
>::ID = Message::MessageID::Unknown;

template<>
const Message::MessageID ErrorControl<
    pegtl::sor<Grammar::NoteSequenceInParentheses, Grammar::NoteRepeatExpression, Grammar::NoteAndDuration>
>::ID = Message::MessageID::Unknown;

template<>
const Message::MessageID ErrorControl<
    pegtl::sor<Grammar::NoteSequenceInParentheses, Grammar::NoteRepeatEachExpression, Grammar::NoteRepeatExpression, Grammar::NoteAndDuration>
>::ID = Message::MessageID::Unknown;

template<>
const Message::MessageID ErrorControl<Grammar::PhraseName>::ID = Message::MessageID::Unknown;

} // namespace Parser

} // namespace YAMML
