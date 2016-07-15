
#pragma once

#include <string>
#include <utility>

#include <pegtl.hh>
#include <pegtl/contrib/unescape.hh>

#include <ast/literal.hpp>

#include "parser_literal.hpp"

namespace YAMML
{

namespace Parser
{

class ValueState
{
public:
    template<typename TParentState, typename... TCommonStates>
    void success(TParentState& st, TCommonStates&...)
    {
        st.ASTNode.Value = ASTNode;
    }

    AST::Literal ASTNode;
};

template<typename TRule>
class ValueAction : public pegtl::nothing<TRule>
{
};

template<>
class ValueAction<Grammar::Value>
{
public:
    template<typename TState, typename... TCommonStates>
    static void apply(const pegtl::input& in, TState& st, TCommonStates&...)
    {
        st.ASTNode.Location = {in.line(), in.column()};
    }
};

template<>
class ValueAction<Grammar::SignedInteger>
{
public:
    template<typename TState, typename... TCommonStates>
    static void apply(const pegtl::input& in, TState& st, TCommonStates&...)
    {
        st.ASTNode.Value = std::stol(in.string());
    }
};

template<>
class ValueAction<Grammar::Identifier>
{
public:
    template<typename TState, typename... TCommonStates>
    static void apply(const pegtl::input& in, TState& st, TCommonStates&...)
    {
        st.ASTNode.Value = in.string();
    }
};

template<typename TRule>
class EscapeAction : public pegtl::nothing<TRule>
{
};

template<>
class EscapeAction<Grammar::AnyChar> : public pegtl::unescape::append_all
{
};

template<>
class EscapeAction<Grammar::EscapeX2> : public pegtl::unescape::unescape_x
{
};

template<>
class EscapeAction<Grammar::EscapeU4> : public pegtl::unescape::unescape_u
{
};

template<>
class EscapeAction<Grammar::EscapeU8> : public pegtl::unescape::unescape_u
{
};

template<>
class EscapeAction<Grammar::EscapeC> : public pegtl::unescape::unescape_c<Grammar::EscapeC, '\'', '"', '?', '\\', '\a', '\b', '\f', '\n', '\r', '\t', '\v'>
{
};

class EscapeState : public pegtl::unescape::state
{
public:
    template<typename TParentState, typename... TCommonStates>
    void success(TParentState& st, TCommonStates&...)
    {
        st.ASTNode.Value = std::move(unescaped);
    }
};

} // namespace Parser

} // namespace YAMML
