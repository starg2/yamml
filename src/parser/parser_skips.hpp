
#pragma once

#include <pegtl.hh>

namespace YAMML
{

namespace Parser
{

namespace Grammar
{

class SingleLineComment : public pegtl::seq<pegtl::string<'/', '/'>, pegtl::until<pegtl::eolf>>
{
};

class BlockComment : public pegtl::if_must<pegtl::string<'/', '*'>, pegtl::until<pegtl::string<'*', '/'>>>
{
};

class Separator : public pegtl::sor<pegtl::space, SingleLineComment, BlockComment>
{
};

class Separators : public pegtl::star<Separator>
{
};

} // namespace Grammar

} // namespace Parser

} // namespace YAMML
