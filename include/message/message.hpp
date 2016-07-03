
#pragma once

#include <string>
#include <vector>

#include <ast/sourcelocation.hpp>
#include <message/id.hpp>
#include <message/kind.hpp>

namespace YAMML
{

namespace Message
{

class MessageItem final
{
public:
    MessageKind Kind;
    MessageID ID;

    std::string SourceName;
    AST::SourceLocation Location;
    std::vector<std::string> Arguments;
};

} // namespace Message

} // namespace YAMML
