
#pragma once

#include <string>
#include <utility>

#include <pegtl.hh>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4715)  // not all control paths return a value
#endif

#include <pegtl/contrib/unescape.hh>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include <ast/literal.hpp>

#include "action.hpp"
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
class ValueAction<Grammar::Value> : public AssignLocationAction
{
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
class EscapeAction<Grammar::AnyChar>
{
public:
    template<typename TState, typename... TCommonStates>
    static void apply(const pegtl::input& in, TState& st, TCommonStates&...)
    {
        pegtl::unescape::append_all::apply(in, st);
    }
};

template<>
class EscapeAction<Grammar::EscapeX2>
{
public:
    template<typename TState, typename... TCommonStates>
    static void apply(const pegtl::input& in, TState& st, TCommonStates&...)
    {
        pegtl::unescape::unescape_x::apply(in, st);
    }
};

template<>
class EscapeAction<Grammar::EscapeU4>
{
public:
    template<typename TState, typename... TCommonStates>
    static void apply(const pegtl::input& in, TState& st, TCommonStates&...)
    {
        pegtl::unescape::unescape_u::apply(in, st);
    }
};

template<>
class EscapeAction<Grammar::EscapeU8>
{
public:
    template<typename TState, typename... TCommonStates>
    static void apply(const pegtl::input& in, TState& st, TCommonStates&...)
    {
        pegtl::unescape::unescape_u::apply(in, st);
    }
};

template<>
class EscapeAction<Grammar::EscapeC>
{
public:
    template<typename TState, typename... TCommonStates>
    static void apply(const pegtl::input& in, TState& st, TCommonStates&...)
    {
        pegtl::unescape::unescape_c<Grammar::EscapeC, '\'', '"', '?', '\\', '\a', '\b', '\f', '\n', '\r', '\t', '\v'>::apply(in, st);
    }
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
