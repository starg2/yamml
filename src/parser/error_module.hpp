
#pragma once

#include <message/id.hpp>

#include "error_control.hpp"
#include "parser_module.hpp"

namespace YAMML
{

namespace Parser
{

template<>
const Message::MessageID ErrorControl<Grammar::TrapNeitherCompositionNorPhrase>::ID = Message::MessageID::GrammarTrapNeitherCompositionNorPhrase;

template<>
const Message::MessageID ErrorControl<Grammar::PhrasesAndCompositions>::ID = Message::MessageID::GrammarPhrasesAndCompositions;

template<>
const Message::MessageID ErrorControl<pegtl::eof>::ID = Message::MessageID::GrammarEOF;

} // namespace Parser

} // namespace YAMML
