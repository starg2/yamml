
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
        if (ast.Arguments.size() != 2)
        {
            ThrowMessage(
                Message::MessageID::WrongNumberOfCommandArguments,
                ast.Location,
                {"panpot", std::to_string(ast.Arguments.size()), "2"}
            );
        }

        ValidateSingleArgument(ast, 0);
        ValidateSingleArgument(ast, 1);

        auto channel = boost::get<long>(ast.Arguments[0].Value);

        if (!(0 <= channel && channel < MIDI::TrackNumberLimit))
        {
            ThrowMessage(
                Message::MessageID::TrackNumberIsOutOfPreferredRange,
                ast.Location,
                {std::to_string(channel), std::to_string(MIDI::TrackNumberLimit)}
            );
        }

        auto panpot = boost::get<long>(ast.Arguments[1].Value);

        if (!(0 <= panpot && panpot < 128))
        {
            ThrowMessage(
                Message::MessageID::InvalidPanpot,
                ast.Location,
                {std::to_string(panpot)}
            );
        }
    }

private:
    void ValidateSingleArgument(const AST::Command& ast, std::size_t index)
    {
        if (ast.Arguments[index].Value.type() != typeid(long))
        {
            ThrowMessage(
                Message::MessageID::WrongTypeOfCommandArgument,
                ast.Location,
                {"panpot", std::to_string(index + 1), "int"}
            );
        }
    }

    IIR2MIDICompiler* m_pCompiler;
};

std::unique_ptr<ICommandProcessor> CreatePanpotCommandProcessor(IIR2MIDICompiler* pCompiler)
{
    return std::make_unique<PanpotCommandProcessor>(pCompiler);
}

} // namespace IR2MIDI

} // namespace YAMML
