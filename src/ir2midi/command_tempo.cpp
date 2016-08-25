
#include <memory>
#include <string>
#include <typeinfo>

#include <boost/variant.hpp>

#include <ir2midi/command.hpp>
#include <message/id.hpp>
#include <midi/event.hpp>

#include "command_tempo.hpp"

namespace YAMML
{

namespace IR2MIDI
{

class TempoCommandProcessor final : public ICommandProcessor
{
public:
    explicit TempoCommandProcessor(IIR2MIDICompiler* pCompiler) : m_pCompiler(pCompiler)
    {
    }

    virtual ~TempoCommandProcessor() = default;

    virtual IIR2MIDICompiler* GetCompiler() override
    {
        return m_pCompiler;
    }

    virtual void Process(const AST::Command& ast) override
    {
        ValidateArguments(ast);

        unsigned int usecPerQuater = 60 * 1'000'000 / boost::get<long>(ast.Arguments[0].Value);

        GetCompiler()->GetTrackContext(0).PushEvent(
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
    }

    void ValidateArguments(const AST::Command& ast)
    {
        if (ast.Arguments.size() != 1)
        {
            ThrowMessage(
                Message::MessageID::WrongNumberOfCommandArguments,
                ast.Location,
                {"tempo", std::to_string(ast.Arguments.size()), "1"}
            );
        }

        if (ast.Arguments[0].Value.type() != typeid(long))
        {
            ThrowMessage(
                Message::MessageID::WrongTypeOfCommandArgument,
                ast.Location,
                {"tempo", "1", "int"}
            );
        }

        auto tempo = boost::get<long>(ast.Arguments[0].Value);

        if (tempo <= 0)
        {
            ThrowMessage(
                Message::MessageID::InvalidTempo,
                ast.Location,
                {std::to_string(tempo)}
            );
        }
    }

private:
    IIR2MIDICompiler* m_pCompiler;
};

std::unique_ptr<ICommandProcessor> CreateTempoCommandProcessor(IIR2MIDICompiler* pCompiler)
{
    return std::make_unique<TempoCommandProcessor>(pCompiler);
}

} // namespace IR2MIDI

} // namespace YAMML
