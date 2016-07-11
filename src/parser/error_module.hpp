
#pragma once

#include <message/id.hpp>

#include "error_control.hpp"
#include "parser_module.hpp"

namespace YAMML
{

namespace Parser
{

template<>
const Message::MessageID ErrorControl<Grammar::PhrasesAndCompositions>::ID = Message::MessageID::Unknown;

} // namespace Parser

} // namespace YAMML
