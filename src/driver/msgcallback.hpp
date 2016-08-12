
#pragma once

#include <string>
#include <unordered_map>

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
    std::unordered_map<Message::MessageKind, std::string> m_KindTextMap;
    std::unordered_map<Message::MessageID, std::string> m_IDTextMap;
    IStdErrWriter* m_pStdErrWriter;
};

} // namespace Driver

} // namespace YAMML
