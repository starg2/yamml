
#pragma once

#include <string>

#include <pegtl.hh>
#include <pegtl/internal/demangle.hh>

#include <exceptions/parseexception.hpp>

#include "parser_literal.hpp"

namespace YAMML
{

namespace Parser
{

class TokenForErrorMessage : public pegtl::sor<Grammar::Identifier, Grammar::SignedInteger, pegtl::utf8::any>
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
void ThrowParseException(const std::string& ruleName, const TInput& in, Message::MessageID id)
{
    std::string tokenForErrorMsg;

    bool matched = pegtl::parse<TokenForErrorMessageAndSeparators, TokenForErrorMessageAction>(
        in.begin(),
        in.end(),
        in.source(),
        tokenForErrorMsg
    );

    throw Exceptions::ParseException(
        ruleName,
        in.source(),
        in.line(),
        in.column(),
        matched ? tokenForErrorMsg : "<EOF>",
        id
    );
}

template<typename TRule>
class ErrorControl : public pegtl::normal<TRule>
{
public:
    static const Message::MessageID ID;

    template<typename TInput, typename... TStates>
    static void raise(const TInput& in, TStates&&...)
    {
        ThrowParseException(pegtl::internal::demangle<TRule>(), in, ID);
    }
};

} // namespace Parser

} // namespace YAMML
