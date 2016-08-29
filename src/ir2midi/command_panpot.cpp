
#include <memory>
#include <string>
#include <typeinfo>

#include <boost/variant.hpp>

#include <ir2midi/command.hpp>
#include <message/id.hpp>
#include <midi/event.hpp>
#include <midi/limits.hpp>

#include "command_panpot.hpp"

namespace YAMML
{

namespace IR2MIDI
{

class PanpotCommandProcessor final : public ICommandProcessor
{
public:
    explicit PanpotCommandProcessor(IIR2MIDICompiler* pCompiler) : m_pCompiler(pCompiler)
    {
    }

    virtual ~PanpotCommandProcessor() = default;

    virtual IIR2MIDICompiler* GetCompiler() override
    {
        return m_pCompiler;
    }

    virtual std::string GetCommandName() const override
    {
        return "panpot";
    }

    virtual void Process(const AST::Command& ast) override
    {
        ValidateArguments(ast);

        auto channel = boost::get<long>(ast.Arguments[0].Value);
        auto panpot = boost::get<long>(ast.Arguments[1].Value);

        GetCompiler()->GetTrackContext(channel).PushEvent(
            0,
            MIDI::ControlChange{channel, MIDI::ControllerNumber::Panpot, panpot}
        );
    }

    void ValidateArguments(const AST::Command& ast)
    {
        ValidateArgCount(ast, 2);
        ValidateArgType(ast, 0, typeid(long));
        ValidateArgType(ast, 1, typeid(long));
        ValidateArgChannel(ast, 0);
        ValidateArgMIDIValue(ast, 1, Message::MessageID::InvalidPanpot);
    }

private:
    IIR2MIDICompiler* m_pCompiler;
};

std::unique_ptr<ICommandProcessor> CreatePanpotCommandProcessor(IIR2MIDICompiler* pCompiler)
{
    return std::make_unique<PanpotCommandProcessor>(pCompiler);
}

} // namespace IR2MIDI

} // namespace YAMML
