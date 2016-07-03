
#include <utility>

#include <parser/parser.hpp>

namespace YAMML
{

namespace Parser
{

YAMMLParser::YAMMLParser(std::string name, std::string source)
    : m_Name(std::move(name)), m_Source(std::move(source))
{
}

YAMMLParser::YAMMLParser(std::string name, std::string source, std::function<CallbackFunctionType> callback)
    : m_Name(std::move(name)), m_Source(std::move(source)), m_Callback(std::move(callback))
{
}

bool YAMMLParser::Parse()
{

    return false;
}

boost::optional<AST::Module>& YAMMLParser::GetAST()
{
    return m_AST;
}

const boost::optional<AST::Module>& YAMMLParser::GetAST() const
{
    return m_AST;
}

std::vector<Message::MessageItem>& YAMMLParser::GetMessages()
{
    return m_Messages;
}

const std::vector<Message::MessageItem>& YAMMLParser::GetMessages() const
{
    return m_Messages;
}

} // namespace Parser

} // namespace YAMML
