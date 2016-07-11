
#pragma once

#include <functional>
#include <string>
#include <vector>

#include <ast/module.hpp>
#include <message/message.hpp>

#include <boost/optional.hpp>

namespace YAMML
{

namespace Parser
{

class YAMMLParser final
{
public:
    using CallbackFunctionType = bool(const Message::MessageItem&);

    YAMMLParser(std::string name, std::string source);
    YAMMLParser(std::string name, std::string source, std::function<CallbackFunctionType> callback);

    YAMMLParser(const YAMMLParser&) = delete;
    YAMMLParser& operator=(const YAMMLParser&) = delete;

    YAMMLParser(YAMMLParser&&) = delete;
    YAMMLParser& operator=(YAMMLParser&&) = delete;

    ~YAMMLParser() = default;

    bool Parse();
    void AddMessage(Message::MessageItem msg);

    boost::optional<AST::Module>& GetAST();
    const boost::optional<AST::Module>& GetAST() const;
    
    std::vector<Message::MessageItem>& GetMessages();
    const std::vector<Message::MessageItem>& GetMessages() const;

private:
    const std::string m_Name;
    const std::string m_Source;
    const std::function<CallbackFunctionType> m_Callback;
    std::vector<Message::MessageItem> m_Messages;
    boost::optional<AST::Module> m_AST;
};

} // namespace Parser

} // namespace YAMML
