
#include <stdexcept>
#include <string>
#include <vector>

#include "format.hpp"
#include "msgcallback.hpp"


namespace YAMML
{

namespace Driver
{

namespace
{

const std::string ICEMessage =
    "An internal compiler error has occurred during the compilation process. "
    "Please submit a bug report with full source code if appropriate.";

}

MessagePrinter::MessagePrinter(IStdErrWriter* pStdErrWriter)
    : m_KindTextMap{
        {Message::MessageKind::FetalError, "fetal error"},
        {Message::MessageKind::Error, "error"},
        {Message::MessageKind::Warning, "warning"},
        {Message::MessageKind::Note, "note"}
    },
    m_IDTextMap{
        {Message::MessageID::Unknown, ICEMessage},
        {Message::MessageID::UnknownInPhrase2IR, ICEMessage + " (Phrase2IR: Phrase = '{0}', Message = '{1}')"},
        {Message::MessageID::UnknownInComposition2IR, ICEMessage + " (Composition2IR: Composition = '{0}', Message = '{1}')"},
        {Message::MessageID::UnknownInIR2MIDI, ICEMessage + " (IR2MIDI: Message = '{0}')"},
        {Message::MessageID::UnprocessedCommand, ICEMessage + " (Unprocessed command in '{0}')"},
        {Message::MessageID::UnprocessedAttribute, ICEMessage + " (Unprocessed attribute in '{0}')"},
        {Message::MessageID::DuplicatedCompositionName, "composition named '{0}' is already defined."},
        {Message::MessageID::DuplicatedPhraseName, "phrase named '{0}' is already defined."},
        {Message::MessageID::NoSuchPhraseName, "cannot find a phrase named '{0}'."},
        {Message::MessageID::NoSuchCompositionName, "cannot find a composition named '{0}'."},
        {Message::MessageID::TrackNumberIsOutOfSafeRange, "track number '{0}' is out of range. (must be 0 <= # < {1})"},
        {Message::MessageID::TrackNumberIsOutOfPreferredRange, "track number '{0}' is out of range. (must be 0 <= # < {1})"},
        {Message::MessageID::TooLargeRepeatCount, "repeat count '{0}' is too large. (must be <= {1})"}
    },
    m_pStdErrWriter{pStdErrWriter}
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
