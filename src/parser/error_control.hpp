
#pragma once

#include <pegtl.hh>
#include <pegtl/internal/demangle.hh>

#include <exceptions/parseexception.hpp>

namespace YAMML
{

namespace Parser
{

template<typename TInput>
void ThrowParseException(const std::string& ruleName, const TInput& in, Message::MessageID id)
{
    throw Exceptions::ParseException(ruleName, in.source(), in.line(), in.column(), id);
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
