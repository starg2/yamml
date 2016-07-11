
#pragma once

#include <message/id.hpp>

#include "error_control.hpp"
#include "parser_literal.hpp"

namespace YAMML
{

namespace Parser
{

template<>
const Message::MessageID ErrorControl<Grammar::Separators>::ID = Message::MessageID::Unknown;

template<>
const Message::MessageID ErrorControl<Grammar::Separator>::ID = Message::MessageID::Unknown;

template<>
const Message::MessageID ErrorControl<pegtl::eof>::ID = Message::MessageID::Unknown;

template<>
const Message::MessageID ErrorControl<Grammar::UntilBlockCommentEnd>::ID = Message::MessageID::Unknown;

template<>
const Message::MessageID ErrorControl<Grammar::Identifier>::ID = Message::MessageID::Unknown;

template<>
const Message::MessageID ErrorControl<Grammar::Escape>::ID = Message::MessageID::Unknown;

template<>
const Message::MessageID ErrorControl<Grammar::AnyChar>::ID = Message::MessageID::Unknown;

template<>
const Message::MessageID ErrorControl<pegtl::not_at<pegtl::digit>>::ID = Message::MessageID::Unknown;

template<>
const Message::MessageID ErrorControl<pegtl::xdigit>::ID = Message::MessageID::Unknown;

template<>
const Message::MessageID ErrorControl<pegtl::until<pegtl::one<'"'>, Grammar::CharcterOrEscapeSequence>>::ID = Message::MessageID::Unknown;

template<>
const Message::MessageID ErrorControl<pegtl::until<pegtl::one<'\''>, Grammar::CharcterOrEscapeSequence>>::ID = Message::MessageID::Unknown;

} // namespace Parser

} // namespace YAMML
