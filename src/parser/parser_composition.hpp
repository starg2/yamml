
#pragma once

#include <pegtl.hh>

#include "parser_attribute.hpp"
#include "parser_common.hpp"
#include "parser_literal.hpp"
#include "parser_skips.hpp"

namespace YAMML
{

namespace Parser
{

namespace Grammar
{

class CommandArgument : public Value
{
};

class Command : public pegtl::seq<Identifier, pegtl::pad_opt<pegtl::list_must<CommandArgument, pegtl::one<','>, Separator>, Separator>, pegtl::one<';'>>
{
};

class TrackItem : public pegtl::seq<AttributeOptionalSequence, pegtl::pad_opt<Identifier, Separator>, pegtl::one<';'>>
{
};

class SpaceSeparatedOptionalTrackItems : public pegtl::star<pegtl::pad<TrackItem, Separator>>
{
};

class TrackBlock
    : public pegtl::seq<
        AttributeOptionalSequence,
        pegtl::if_must<
            UnsignedInteger,
            BlockBegin,
            SpaceSeparatedOptionalTrackItems,
            BlockEnd
        >
    >
{
};

class OneOrMoreTrackBlocks : public pegtl::plus<TrackBlock>
{
};

class TrackListBlock
    : public pegtl::seq<
        AttributeOptionalSequence,
        pegtl::if_must<
            pegtl_string_t("tracks"),
            BlockBegin,
            OneOrMoreTrackBlocks,
            BlockEnd
        >
    >
{
};

class CommandsAndTrackListBlocks : public pegtl::star<pegtl::sor<pegtl::pad<Command, Separator>, TrackListBlock>>
{
};

class Composition
    : public pegtl::seq<
        AttributeOptionalSequence,
        pegtl::if_must<
            pegtl_string_t("composition"),
            Separator,
            Identifier,
            BlockBegin,
            CommandsAndTrackListBlocks,
            BlockEnd
        >
    >
{
};

} // namespace Grammar

} // namespace Parser

} // namespace YAMML
