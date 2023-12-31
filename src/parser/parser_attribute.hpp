
#pragma once

#include <pegtl.hh>

#include "parser_literal.hpp"
#include "parser_skips.hpp"

namespace YAMML
{

namespace Parser
{

namespace Grammar
{

class AttributeKey : public pegtl::seq<pegtl::pad<Identifier, Separator>, pegtl::pad<pegtl::one<'='>, Separator>>
{
};

class AttributeArgument : public pegtl::seq<pegtl::opt<AttributeKey>, pegtl::pad<Value, Separator>>
{
};

class CommaSeparatedOptionalAttributeArguments
    : public pegtl::pad_opt<pegtl::list_must<AttributeArgument, pegtl::one<','>, Separator>, Separator>
{
};

class AttributeArgumentList
    : public pegtl::if_must<pegtl::one<'('>, CommaSeparatedOptionalAttributeArguments, pegtl::one<')'>>
{
};

class AttributeNameAndArguments : public pegtl::seq<Identifier, pegtl::pad_opt<AttributeArgumentList, Separator>>
{
};

class AttributeList : public pegtl::list_must<AttributeNameAndArguments, pegtl::one<','>, Separator>
{
};

class Attribute
    : public pegtl::if_must<
        pegtl::one<'['>,
        AttributeList,
        pegtl::one<']'>
    >
{
};

class AttributeOptionalSequence : public pegtl::pad<pegtl::star<pegtl::pad<Attribute, Separator>>, Separator>
{
};

} // namespace Grammar

} // namespace Parser

} // namespace YAMML
