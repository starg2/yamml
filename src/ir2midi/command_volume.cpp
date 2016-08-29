
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

    virtual std::string GetCommandName() const override
    {
        return "volume";
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
        ValidateArgCount(ast, 2);
        ValidateArgType(ast, 0, typeid(long));
        ValidateArgType(ast, 1, typeid(long));
        ValidateArgChannel(ast, 0);
        ValidateArgMIDIValue(ast, 1, Message::MessageID::InvalidVolume);
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
