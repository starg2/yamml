
#pragma once

#include <stdexcept>
#include <string>
#include <utility>

#include <pegtl.hh>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4715)  // not all control paths return a value
#endif

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#endif

#include <pegtl/contrib/unescape.hh>

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include <ast/literal.hpp>
#include <message/message.hpp>

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
        st.OnParse(ASTNode);
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
    template<typename TState, typename TCompiler, typename... TCommonStates>
    static void apply(const pegtl::input& in, TState& st, TCompiler& compiler, TCommonStates&...)
    {
        try
        {
            st.ASTNode.Value = std::stol(in.string());
        }
        catch (const std::out_of_range&)
        {
            compiler.AddMessage(
                Message::MessageItem{
                    Message::MessageKind::Error,
                    Message::MessageID::IntegerOutOfRange,
                    compiler.GetSourceName(),
                    {in.line(), in.column()},
                    {in.string()}
                }
            );
        }
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
