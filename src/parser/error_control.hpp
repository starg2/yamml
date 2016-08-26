
#pragma once

#include <string>

#include <pegtl.hh>
#include <pegtl/internal/demangle.hh>

#include <message/message.hpp>

#include "parser_literal.hpp"

namespace YAMML
{

namespace Parser
{

class TokenForErrorMessage : public pegtl::sor<Grammar::Identifier, Grammar::UnsignedInteger, pegtl::utf8::any>
{
};

class TokenForErrorMessageAndSeparators : public pegtl::seq<Grammar::Separators, TokenForErrorMessage>
{
};

template<typename TRule>
class TokenForErrorMessageAction : public pegtl::nothing<TRule>
{
};

template<>
class TokenForErrorMessageAction<TokenForErrorMessage>
{
public:
    static void apply(const pegtl::input& in, std::string& st)
    {
        st = in.string();
    }
};

template<typename TInput>
auto CreateParseErrorMessage(Message::MessageKind kind, Message::MessageID id, const TInput& in)
{
    std::string tokenForErrorMsg;

    bool matched = pegtl::parse<TokenForErrorMessageAndSeparators, TokenForErrorMessageAction>(
        in.begin(),
        in.end(),
        in.source(),
        tokenForErrorMsg
    );

    return Message::MessageItem{
        kind,
        id,
        in.source(),
        {in.line(), in.column()},
        {matched ? tokenForErrorMsg : "<EOF>"}
    };
}

template<typename TRule>
class ErrorControl : public pegtl::normal<TRule>
{
public:
    static const Message::MessageID ID;

    template<typename TInput, typename TCurrentState, typename TCompiler, typename... TOtherStates>
    static void raise(const TInput& in, TCurrentState&, TCompiler& compiler, TOtherStates&&...)
    {
        compiler.AddMessage(CreateParseErrorMessage(Message::MessageKind::Error, ID, in));
        ErrorControl::normal::raise(in);
    }
};

} // namespace Parser

} // namespace YAMML
