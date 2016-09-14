
#pragma once

#include <pegtl.hh>

#include "parser_attribute.hpp"
#include "parser_composition.hpp"
#include "parser_phrase.hpp"
#include "parser_skips.hpp"

namespace YAMML
{

namespace Parser
{

namespace Grammar
{

class TrapNeitherCompositionNorPhrase
    : public pegtl::seq<
        pegtl::disable<AttributeOptionalSequence>,
        pegtl::not_at<pegtl::eof>,
        pegtl::raise<TrapNeitherCompositionNorPhrase>
    >
{
};

class PhrasesAndCompositions : public pegtl::star<pegtl::pad<pegtl::sor<Composition, Phrase, TrapNeitherCompositionNorPhrase>, Separator>>
{
};

class Module : public pegtl::must<PhrasesAndCompositions, pegtl::eof>
{
};

} // namespace Grammar

} // namespace Parser

} // namespace YAMML
