
#pragma once

#include <string>

#include <pegtl.hh>

#include <ast/composition.hpp>
#include <ast/module.hpp>

#include "parser_composition.hpp"

namespace YAMML
{

namespace Parser
{

class CompositionState
{
public:
    template<typename... TCommonStates>
    void success(AST::Module& mod, TCommonStates&...)
    {
        mod.Add(ASTNode);
    }

    AST::Composition ASTNode;
};

template<typename TRule>
class CompositionAction : public pegtl::nothing<TRule>
{
};

template<>
class CompositionAction<Grammar::Composition>
{
public:
    template<typename... TCommonStates>
    static void apply(const pegtl::input& in, CompositionState& st, TCommonStates&...)
    {
        st.ASTNode.Location = {in.line(), in.column()};
    }
};

class TrackListBlockState
{
public:
    template<typename TParentState, typename... TCommonStates>
    void success(TParentState& st, TCommonStates&...)
    {
        st.ASTNode.Statements.emplace_back(ASTNode);
    }

    AST::TrackListBlock ASTNode;
};

template<typename TRule>
class TrackListBlockAction : public pegtl::nothing<TRule>
{
};

template<>
class TrackListBlockAction<Grammar::TrackListBlock>
{
public:
    template<typename... TCommonStates>
    static void apply(const pegtl::input& in, TrackListBlockState& st, TCommonStates&...)
    {
        st.ASTNode.Location = {in.line(), in.column()};
    }
};

class TrackBlockState
{
public:
    template<typename TParentState, typename... TCommonStates>
    void success(TParentState& st, TCommonStates&...)
    {
        st.ASTNode.Tracks.push_back(ASTNode);
    }

    AST::TrackBlock ASTNode;
};

template<typename TRule>
class TrackBlockAction : public pegtl::nothing<TRule>
{
};

template<>
class TrackBlockAction<Grammar::TrackListBlock>
{
public:
    template<typename... TCommonStates>
    static void apply(const pegtl::input& in, TrackBlockState& st, TCommonStates&...)
    {
        st.ASTNode.Location = {in.line(), in.column()};
    }
};

template<>
class TrackBlockAction<Grammar::UnsignedInteger>
{
public:
    template<typename... TCommonStates>
    static void apply(const pegtl::input& in, TrackBlockState& st, TCommonStates&...)
    {
        st.ASTNode.TrackNumber = std::stoul(in.string());
    }
};

class TrackItemState
{
public:
    template<typename TParentState, typename... TCommonStates>
    void success(TParentState& st, TCommonStates&...)
    {
        st.ASTNode.Items.push_back(ASTNode);
    }

    AST::TrackItem ASTNode;
};

template<typename TRule>
class TrackItemAction : public pegtl::nothing<TRule>
{
};

template<>
class TrackItemAction<Grammar::TrackItem>
{
public:
    template<typename... TCommonStates>
    static void apply(const pegtl::input& in, TrackItemState& st, TCommonStates&...)
    {
        st.ASTNode.Location = {in.line(), in.column()};
    }
};

template<>
class TrackItemAction<Grammar::Identifier>
{
public:
    template<typename... TCommonStates>
    static void apply(const pegtl::input& in, TrackItemState& st, TCommonStates&...)
    {
        st.ASTNode.PhraseName = in.string();
    }
};

class CommandState
{
public:
    template<typename TParentState, typename... TCommonStates>
    void success(TParentState& st, TCommonStates&...)
    {
        st.ASTNode.Statements.push_back(ASTNode);
    }

    AST::Command ASTNode;
};

template<typename TRule>
class CommandAction : public pegtl::nothing<TRule>
{
};

template<>
class CommandAction<Grammar::Command>
{
public:
    template<typename... TCommonStates>
    static void apply(const pegtl::input& in, CommandState& st, TCommonStates&...)
    {
        st.ASTNode.Location = {in.line(), in.column()};
    }
};

template<>
class CommandAction<Grammar::Identifier>
{
public:
    template<typename... TCommonStates>
    static void apply(const pegtl::input& in, CommandState& st, TCommonStates&...)
    {
        st.ASTNode.Name = in.string();
    }
};

class CommandArgumentState
{
public:
    template<typename TParentState, typename... TCommonStates>
    void success(TParentState& st, TCommonStates&...)
    {
        st.ASTNode.Arguments.push_back(ASTNode);
    }

    AST::Literal ASTNode;
};

template<typename TRule>
class CommandArgumentAction : public pegtl::nothing<TRule>
{
};

template<>
class CommandArgumentAction<Grammar::CommandArgument>
{
public:
    template<typename... TCommonStates>
    static void apply(const pegtl::input& in, CommandArgumentState& st, TCommonStates&...)
    {
        st.ASTNode.Location = {in.line(), in.column()};
    }
};

} // namespace Parser

} // namespace YAMML
