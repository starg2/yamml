
#pragma once

#include <pegtl.hh>

#include <exceptions/parseexception.hpp>
#include <message/id.hpp>

namespace YAMML
{

namespace Parser
{

template<typename TInput>
void ThrowParseException(const TInput& in, Message::MessageID id)
{
    throw Exceptions::ParseException(in.source(), in.line(), in.column(), id);
}

template<typename TRule>
class ErrorControl : public pegtl::normal<TRule>
{
public:
    static const Message::MessageID ID;

    template<typename TInput, typename... TStates>
    static void raise(const TInput& in, TStates&&...)
    {
        ThrowParseException(in, ID);
    }
};

} // namespace Parser

} // namespace YAMML
