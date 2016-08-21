
#pragma once

#include <algorithm>
#include <functional>
#include <utility>
#include <vector>

#include <message/message.hpp>

namespace YAMML
{

namespace Compiler
{

class CompilerBase
{
public:
    using CallbackFunctionType = bool(const Message::MessageItem&);

    CompilerBase() = default;

    template<typename T>
    explicit CompilerBase(T func) : m_Callback(func)
    {
    }

    CompilerBase(const CompilerBase&) = delete;
    CompilerBase& operator=(const CompilerBase&) = delete;

    CompilerBase(CompilerBase&&) = delete;
    CompilerBase& operator=(CompilerBase&&) = delete;

    void AddMessage(const Message::MessageItem& msg)
    {
        m_Messages.push_back(msg);

        if (m_Callback)
        {
            m_Callback(msg);
        }
    }

    void AddMessages(const std::vector<Message::MessageItem>& messages)
    {
        for (auto&& i : messages)
        {
            AddMessage(i);
        }
    }

    const std::vector<Message::MessageItem>& GetMessages() const
    {
        return m_Messages;
    }

    bool HasErrors() const
    {
        return std::find_if(
            GetMessages().begin(),
            GetMessages().end(),
            [] (auto&& i) { return i.Kind == Message::MessageKind::Error || i.Kind == Message::MessageKind::FetalError; }
        ) != GetMessages().end();
    }

private:
    std::function<CallbackFunctionType> m_Callback;
    std::vector<Message::MessageItem> m_Messages;
};

} // namespace Compiler

} // namespace YAMML
