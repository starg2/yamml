
#pragma once

#include <message/id.hpp>

#include "error_control.hpp"
#include "parser_composition.hpp"

namespace YAMML
{

namespace Parser
{

template<>
const Message::MessageID ErrorControl<Grammar::CommandArgument>::ID = Message::MessageID::Unknown;

template<>
const Message::MessageID ErrorControl<Grammar::CommandsAndTrackListBlocks>::ID = Message::MessageID::Unknown;

template<>
const Message::MessageID ErrorControl<Grammar::OneOrMoreTrackBlocks>::ID = Message::MessageID::Unknown;

template<>
const Message::MessageID ErrorControl<Grammar::SpaceSeparatedOptionalTrackItems>::ID = Message::MessageID::Unknown;

} // namespace Parser

} // namespace YAMML
