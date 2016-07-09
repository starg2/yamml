
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
    void success(AST::Module& mod)
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
    static void apply(const pegtl::input& in, CompositionState& st)
    {
        st.ASTNode.Location = {in.line(), in.column()};
    }
};

class TrackListBlockState
{
public:
    template<typename TState>
    void success(TState& st)
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
    static void apply(const pegtl::input& in, TrackListBlockState& st)
    {
        st.ASTNode.Location = {in.line(), in.column()};
    }
};

class TrackBlockState
{
public:
    template<typename TState>
    void success(TState& st)
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
    static void apply(const pegtl::input& in, TrackBlockState& st)
    {
        st.ASTNode.Location = {in.line(), in.column()};
    }
};

template<>
class TrackBlockAction<Grammar::UnsignedInteger>
{
public:
    static void apply(const pegtl::input& in, TrackBlockState& st)
    {
        st.ASTNode.TrackNumber = std::stoul(in.string());
    }
};

class TrackItemState
{
public:
    template<typename TState>
    void success(TState& st)
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
    static void apply(const pegtl::input& in, TrackItemState& st)
    {
        st.ASTNode.Location = {in.line(), in.column()};
    }
};

template<>
class TrackItemAction<Grammar::Identifier>
{
public:
    static void apply(const pegtl::input& in, TrackItemState& st)
    {
        st.ASTNode.PhraseName = in.string();
    }
};

class CommandState
{
public:
    template<typename TState>
    void success(TState& st)
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
    static void apply(const pegtl::input& in, CommandState& st)
    {
        st.ASTNode.Location = {in.line(), in.column()};
    }
};

template<>
class CommandAction<Grammar::Identifier>
{
public:
    static void apply(const pegtl::input& in, CommandState& st)
    {
        st.ASTNode.Name = in.string();
    }
};

class CommandArgumentState
{
public:
    template<typename TState>
    void success(TState& st)
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
    static void apply(const pegtl::input& in, CommandArgumentState& st)
    {
        st.ASTNode.Location = {in.line(), in.column()};
    }
};

} // namespace Parser

} // namespace YAMML
