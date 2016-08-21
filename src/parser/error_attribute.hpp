
#pragma once

#include <message/id.hpp>

#include "error_control.hpp"
#include "parser_attribute.hpp"

namespace YAMML
{

namespace Parser
{

template<>
const Message::MessageID ErrorControl<Grammar::AttributeArgument>::ID = Message::MessageID::GrammarAttributeArgument;

template<>
const Message::MessageID ErrorControl<Grammar::CommaSeparatedOptionalAttributeArguments>::ID = Message::MessageID::GrammarCommaSeparatedOptionalAttributeArguments;

template<>
const Message::MessageID ErrorControl<Grammar::AttributeList>::ID = Message::MessageID::GrammarAttributeList;

template<>
const Message::MessageID ErrorControl<Grammar::AttributeNameAndArguments>::ID = Message::MessageID::GrammarAttributeNameAndArguments;

template<>
const Message::MessageID ErrorControl<pegtl::one<']'>>::ID = Message::MessageID::GrammarEndBracket;

} // namespace Parser

} // namespace YAMML
