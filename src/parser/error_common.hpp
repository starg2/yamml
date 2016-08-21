
#pragma once

#include <message/id.hpp>

#include "error_control.hpp"
#include "parser_common.hpp"

namespace YAMML
{

namespace Parser
{

template<>
const Message::MessageID ErrorControl<Grammar::BlockBegin>::ID = Message::MessageID::GrammarBlockBegin;

template<>
const Message::MessageID ErrorControl<Grammar::BlockEnd>::ID = Message::MessageID::GrammarBlockEnd;

template<>
const Message::MessageID ErrorControl<pegtl::one<')'>>::ID = Message::MessageID::GrammarEndParenthesis;

} // namespace Parser

} // namespace YAMML
