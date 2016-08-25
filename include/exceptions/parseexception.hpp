
#pragma once

#include <string>

#include <exceptions/exception.hpp>
#include <message/id.hpp>

namespace YAMML
{

namespace Exceptions
{

class ParseException : public Exception
{
public:
    ParseException(
        const std::string& ruleName,
        const std::string& source,
        std::size_t line,
        std::size_t column,
        const std::string& nextToken,
        Message::MessageID id
    ) : Exception("ParseException: MessageID=" + std::to_string(static_cast<int>(id))),
        RuleName(ruleName),
        Source(source),
        Line{line},
        Column{column},
        NextToken(nextToken),
        ID{id}
    {
    }

    std::string RuleName;
    std::string Source;
    std::size_t Line;
    std::size_t Column;
    std::string NextToken;
    Message::MessageID ID;
};

} // namespace Exceptions

} // namespace YAMML
