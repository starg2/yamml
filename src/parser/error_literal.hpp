
#pragma once

#include <message/id.hpp>

#include "error_control.hpp"
#include "parser_literal.hpp"

namespace YAMML
{

namespace Parser
{

template<>
const Message::MessageID ErrorControl<Grammar::Separators>::ID = Message::MessageID::GrammarSeparators;

template<>
const Message::MessageID ErrorControl<Grammar::Separator>::ID = Message::MessageID::GrammarSeparator;

template<>
const Message::MessageID ErrorControl<Grammar::UntilBlockCommentEnd>::ID = Message::MessageID::GrammarUntilBlockCommentEnd;

template<>
const Message::MessageID ErrorControl<Grammar::Escape>::ID = Message::MessageID::GrammarEscape;

template<>
const Message::MessageID ErrorControl<pegtl::not_at<pegtl::digit>>::ID = Message::MessageID::GrammarNoDigit;

template<>
const Message::MessageID ErrorControl<pegtl::rep<2, pegtl::xdigit>>::ID = Message::MessageID::GrammarXDigit2;

template<>
const Message::MessageID ErrorControl<pegtl::rep<4, pegtl::xdigit>>::ID = Message::MessageID::GrammarXDigit4;

template<>
const Message::MessageID ErrorControl<pegtl::rep<8, pegtl::xdigit>>::ID = Message::MessageID::GrammarXDigit8;

template<>
const Message::MessageID ErrorControl<pegtl::until<pegtl::one<'"'>, Grammar::CharcterOrEscapeSequence>>::ID = Message::MessageID::GrammarUntilDoubleQuote;

template<>
const Message::MessageID ErrorControl<pegtl::until<pegtl::one<'\''>, Grammar::CharcterOrEscapeSequence>>::ID = Message::MessageID::GrammarUntilSingleQuote;

} // namespace Parser

} // namespace YAMML
