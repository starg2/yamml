
#include <stdexcept>
#include <string>
#include <vector>

#include "format.hpp"
#include "msgcallback.hpp"


namespace YAMML
{

namespace Driver
{

MessagePrinter::MessagePrinter(IStdErrWriter* pStdErrWriter) : m_pStdErrWriter{pStdErrWriter}
{
    if (!pStdErrWriter)
    {
        throw std::invalid_argument("MessagePrinter::MessagePrinter(pStdErrWriter): pStdErrWriter is null");
    }
}

bool MessagePrinter::operator()(const Message::MessageItem& item) const
{
    std::string output = item.SourceName;

    if (item.Location.Line != 0)
    {
        output += '(' + std::to_string(item.Location.Line) + ')';
    }

    output += ": " + m_KindTextMap.at(item.Kind) + ": " + FormatText(m_IDTextMap.at(item.ID), item.Arguments);

    m_pStdErrWriter->WriteLine(output);
    return true;
}

} // namespace Driver

} // namespace YAMML
