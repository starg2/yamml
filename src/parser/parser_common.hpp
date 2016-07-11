
#pragma once

#include <pegtl.hh>

#include "parser_skips.hpp"

namespace YAMML
{

namespace Parser
{

namespace Grammar
{

class BlockBegin : public pegtl::pad<pegtl::one<'{'>, Separator>
{
};

class BlockEnd : public pegtl::pad<pegtl::one<'}'>, Separator>
{
};

} // namespace Grammar

} // namespace Parser

} // namespace YAMML
