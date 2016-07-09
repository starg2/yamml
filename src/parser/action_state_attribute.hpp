
#pragma once

#include <pegtl.hh>

#include <ast/attribute.hpp>

#include "parser_attribute.hpp"


namespace YAMML
{

namespace Parser
{

class AttributeState
{
public:
    template<typename TState>
    void success(TState& st)
    {
        st.ASTNode.Attributes.push_back(ASTNode);
    }

    AST::Attribute ASTNode;
};

template<typename TRule>
class AttributeAction : public pegtl::nothing<TRule>
{
};

template<>
class AttributeAction<Grammar::Attribute>
{
public:
    static void apply(const pegtl::input& in, AttributeState& st)
    {
        st.ASTNode.Location = {in.line(), in.column()};
    }
};

template<>
class AttributeAction<Grammar::Identifier>
{
public:
    static void apply(const pegtl::input& in, AttributeState& st)
    {
        st.ASTNode.Name = in.string();
    }
};

class AttributeArgumentState
{
public:
    template<typename TState>
    void success(TState& st)
    {
        st.ASTNode.Arguments.push_back(ASTNode);
    }

    AST::AttributeArgument ASTNode;
};

template<typename TRule>
class AttributeArgumentAction : public pegtl::nothing<TRule>
{
};

template<>
class AttributeArgumentAction<Grammar::AttributeArgument>
{
public:
    static void apply(const pegtl::input& in, AttributeArgumentState& st)
    {
        st.ASTNode.Location = {in.line(), in.column()};
    }
};

template<typename TRule>
class AttributeKeyAction : public pegtl::nothing<TRule>
{
};

template<>
class AttributeKeyAction<Grammar::Identifier>
{
public:
    static void apply(const pegtl::input& in, AttributeArgumentState& st)
    {
        st.ASTNode.Name = in.string();
    }
};

} // namespace Parser

} // namespace YAMML
