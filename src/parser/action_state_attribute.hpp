
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
    template<typename TParentState, typename... TCommonStates>
    void success(TParentState& st, TCommonStates&...)
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
    template<typename... TCommonStates>
    static void apply(const pegtl::input& in, AttributeState& st, TCommonStates&...)
    {
        st.ASTNode.Location = {in.line(), in.column()};
    }
};

template<>
class AttributeAction<Grammar::Identifier>
{
public:
    template<typename... TCommonStates>
    static void apply(const pegtl::input& in, AttributeState& st, TCommonStates&...)
    {
        st.ASTNode.Name = in.string();
    }
};

class AttributeArgumentState
{
public:
    template<typename TParentState, typename... TCommonStates>
    void success(TParentState& st, TCommonStates&...)
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
    template<typename... TCommonStates>
    static void apply(const pegtl::input& in, AttributeArgumentState& st, TCommonStates&...)
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
    template<typename... TCommonStates>
    static void apply(const pegtl::input& in, AttributeArgumentState& st, TCommonStates&...)
    {
        st.ASTNode.Name = in.string();
    }
};

} // namespace Parser

} // namespace YAMML
