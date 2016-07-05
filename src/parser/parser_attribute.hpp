
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

class AttributeArgumentList
    : public pegtl::if_must<pegtl::one<'('>, pegtl::pad_opt<pegtl::list_must<Value, pegtl::one<','>, Separator>, Separator>, pegtl::one<')'>>
{
};

class Attribute
    : public pegtl::if_must<
        pegtl::one<'['>,
        pegtl::pad<
            pegtl::list_must<
                pegtl::seq<Identifier, pegtl::pad_opt<AttributeArgumentList, Separator>>,
                pegtl::one<','>,
                Separator
            >,
            Separator
        >,
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
