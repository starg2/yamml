
#include <boost/variant.hpp>

#include "command_tempo.hpp"

#include <midi/event.hpp>

namespace YAMML
{

namespace IR2MIDI
{

std::vector<Message::MessageItem> ProcessTempo(IIR2MIDICompiler* pCompiler, const AST::Command& ast)
{
    std::vector<Message::MessageItem> messages;

    if (ast.Arguments.size() != 1)
    {
        messages.push_back(
            Message::MessageItem{
                Message::MessageKind::Error,
                Message::MessageID::WrongNumberOfCommandArguments,
                pCompiler->GetSourceName(),
                ast.Location,
                {"tempo", std::to_string(ast.Arguments.size()), "1"}
            }
        );

        return messages;
    }

    if (ast.Arguments[0].Value.type() != typeid(long))
    {
        messages.push_back(
            Message::MessageItem{
                Message::MessageKind::Error,
                Message::MessageID::WrongTypeOfCommandArgument,
                pCompiler->GetSourceName(),
                ast.Location,
                {"tempo", "1", "int"}
            }
        );

        return messages;
    }

    auto tempo = boost::get<long>(ast.Arguments[0].Value);

    if (tempo <= 0)
    {
        messages.push_back(
            Message::MessageItem{
                Message::MessageKind::Error,
                Message::MessageID::InvalidTempo,
                pCompiler->GetSourceName(),
                ast.Location,
                {std::to_string(tempo)}
            }
        );

        return messages;
    }

    unsigned int usecPerQuater = 60 * 1'000'000 / tempo;

    pCompiler->GetTrackContext(0).PushEvent(
        0,
        MIDI::MetaEvent{
            MIDI::MetaEventKind::SetTempo,
            {
                static_cast<std::uint8_t>((usecPerQuater & 0xFF0000) >> 16),
                static_cast<std::uint8_t>((usecPerQuater & 0xFF00) >> 8),
                static_cast<std::uint8_t>(usecPerQuater & 0xFF)
            }
        }
    );

    return messages;
}

} // namespace IR2MIDI

} // namespace YAMML
