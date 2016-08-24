
#pragma once

#include <pegtl.hh>

#include "parser_composition.hpp"
#include "parser_phrase.hpp"
#include "parser_skips.hpp"

namespace YAMML
{

namespace Parser
{

namespace Grammar
{

class PhrasesAndCompositions : public pegtl::star<pegtl::pad<pegtl::sor<Composition, Phrase>, Separator>>
{
};

class Module : public pegtl::must<PhrasesAndCompositions, pegtl::eof>
{
};

} // namespace Grammar

} // namespace Parser

} // namespace YAMML
