
#pragma once

#include <map>
#include <string>

#include <message/message.hpp>

#include "stderrwriter.hpp"

namespace YAMML
{

namespace Driver
{

class MessagePrinter final
{
public:
    explicit MessagePrinter(IStdErrWriter* pStdErrWriter);

    bool operator()(const Message::MessageItem& item) const;

private:
    std::map<Message::MessageKind, std::string> m_KindTextMap;
    std::map<Message::MessageID, std::string> m_IDTextMap;
    IStdErrWriter* m_pStdErrWriter;
};

} // namespace Driver

} // namespace YAMML
