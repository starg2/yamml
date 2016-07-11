
#pragma once

#include <message/id.hpp>

#include "error_control.hpp"
#include "parser_attribute.hpp"

namespace YAMML
{

namespace Parser
{

template<>
const Message::MessageID ErrorControl<Grammar::AttributeArgument>::ID = Message::MessageID::Unknown;

template<>
const Message::MessageID ErrorControl<Grammar::CommaSeparatedOptionalAttributeArguments>::ID = Message::MessageID::Unknown;

template<>
const Message::MessageID ErrorControl<Grammar::AttributeList>::ID = Message::MessageID::Unknown;

template<>
const Message::MessageID ErrorControl<Grammar::AttributeNameAndArguments>::ID = Message::MessageID::Unknown;

template<>
const Message::MessageID ErrorControl<pegtl::one<']'>>::ID = Message::MessageID::Unknown;

} // namespace Parser

} // namespace YAMML
