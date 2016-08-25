
#include <memory>
#include <typeinfo>

#include <ast/composition.hpp>
#include <ir2midi/command.hpp>
#include <ir2midi/context.hpp>
#include <message/id.hpp>
#include <midi/event.hpp>
#include <midi/limits.hpp>

#include "command_volume.hpp"


namespace YAMML
{

namespace IR2MIDI
{

class VolumeCommandProcessor final : public ICommandProcessor
{
public:
    explicit VolumeCommandProcessor(IIR2MIDICompiler* pCompiler) : m_pCompiler(pCompiler)
    {
    }

    virtual ~VolumeCommandProcessor() = default;

    virtual IIR2MIDICompiler* GetCompiler() override
    {
        return m_pCompiler;
    }

    virtual void Process(const AST::Command& ast) override
    {
        ValidateArguments(ast);

        auto channel = boost::get<long>(ast.Arguments[0].Value);
        auto volume = boost::get<long>(ast.Arguments[1].Value);

        GetCompiler()->GetTrackContext(channel).PushEvent(
            0,
            MIDI::ControlChange{channel, MIDI::ControllerNumber::MainVolume, volume}
        );
    }

    void ValidateArguments(const AST::Command& ast)
    {
        if (ast.Arguments.size() != 2)
        {
            ThrowMessage(
                Message::MessageID::WrongNumberOfCommandArguments,
                ast.Location,
                {"volume", std::to_string(ast.Arguments.size()), "2"}
            );
        }

        if (ast.Arguments[0].Value.type() != typeid(long))
        {
            ThrowMessage(
                Message::MessageID::WrongTypeOfCommandArgument,
                ast.Location,
                {"volume", "1", "int"}
            );
        }

        if (ast.Arguments[1].Value.type() != typeid(long))
        {
            ThrowMessage(
                Message::MessageID::WrongTypeOfCommandArgument,
                ast.Location,
                {"volume", "2", "int"}
            );
        }

        auto channel = boost::get<long>(ast.Arguments[0].Value);

        if (!(0 <= channel && channel < MIDI::TrackNumberLimit))
        {
            ThrowMessage(
                Message::MessageID::TrackNumberIsOutOfPreferredRange,
                ast.Location,
                {std::to_string(channel), std::to_string(MIDI::TrackNumberLimit)}
            );
        }

        auto volume = boost::get<long>(ast.Arguments[1].Value);

        if (!(0 <= volume && volume < 128))
        {
            ThrowMessage(
                Message::MessageID::InvalidVolume,
                ast.Location,
                {std::to_string(channel)}
            );
        }
    }

private:
    IIR2MIDICompiler* m_pCompiler;
};

std::unique_ptr<ICommandProcessor> CreateVolumeCommandProcessor(IIR2MIDICompiler* pCompiler)
{
    return std::make_unique<VolumeCommandProcessor>(pCompiler);
}

} // namespace IR2MIDI

} // namespace YAMML
