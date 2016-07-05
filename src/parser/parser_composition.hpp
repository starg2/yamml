
#pragma once

#include <pegtl.hh>

#include "parser_attribute.hpp"
#include "parser_literal.hpp"
#include "parser_skips.hpp"

namespace YAMML
{

namespace Parser
{

namespace Grammar
{

class Command : public pegtl::seq<Identifier, pegtl::pad_opt<pegtl::list_must<Value, pegtl::one<','>, Separator>, Separator>, pegtl::one<';'>>
{
};

class TrackItem : public pegtl::seq<AttributeOptionalSequence, pegtl::pad_opt<Identifier, Separator>, pegtl::one<';'>>
{
};

class TrackBlock
    : public pegtl::seq<
        AttributeOptionalSequence,
        pegtl::if_must<
            UnsignedInteger,
            pegtl::pad<pegtl::one<'{'>, Separator>,
            pegtl::star<TrackItem>,
            pegtl::pad<pegtl::one<'}'>, Separator>
        >
    >
{
};

class TrackListBlock
    : public pegtl::seq<
        AttributeOptionalSequence,
        pegtl::if_must<
            pegtl_string_t("tracks"),
            pegtl::pad<pegtl::one<'{'>, Separator>,
            pegtl::star<TrackBlock>,
            pegtl::pad<pegtl::one<'}'>, Separator>
        >
    >
{
};

class Composition
    : public pegtl::seq<
        AttributeOptionalSequence,
        pegtl::if_must<
            pegtl_string_t("composition"),
            Separator,
            Identifier,
            pegtl::pad<pegtl::one<'{'>, Separator>,
            pegtl::star<pegtl::sor<pegtl::pad<Command, Separator>, TrackListBlock>>,
            pegtl::pad<pegtl::one<'}'>, Separator>
        >
    >
{
};

} // namespace Grammar

} // namespace Parser

} // namespace YAMML
