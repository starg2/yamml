
#pragma once

#include <message/id.hpp>

#include "error_control.hpp"
#include "parser_composition.hpp"

namespace YAMML
{

namespace Parser
{

template<>
const Message::MessageID ErrorControl<Grammar::CommandArgument>::ID = Message::MessageID::GrammarCommandArgument;

template<>
const Message::MessageID ErrorControl<Grammar::CommandsAndTrackListBlocks>::ID = Message::MessageID::GrammarCommandsAndTrackListBlocks;

template<>
const Message::MessageID ErrorControl<Grammar::OneOrMoreTrackBlocks>::ID = Message::MessageID::GrammarOneOrMoreTrackBlocks;

template<>
const Message::MessageID ErrorControl<Grammar::SpaceSeparatedOptionalTrackItems>::ID = Message::MessageID::GrammarSpaceSeparatedOptionalTrackItems;

template<>
const Message::MessageID ErrorControl<Grammar::CompositionName>::ID = Message::MessageID::GrammarCompositionName;

} // namespace Parser

} // namespace YAMML
