
#pragma once

#include <string>

#include <exceptions/exception.hpp>
#include <message/message.hpp>

namespace YAMML
{

namespace Exceptions
{

class MessageException : public Exception
{
public:
    MessageException(const Message::MessageItem& item)
        : Exception("MessageException: MessageID=" + std::to_string(static_cast<int>(item.ID))), Item(item)
    {
    }

    Message::MessageItem Item;
};

} // namespace Exceptions

} // namespace YAMML
