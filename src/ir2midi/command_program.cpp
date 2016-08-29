
#include <memory>
#include <string>
#include <typeinfo>

#include <boost/variant.hpp>

#include <exceptions/invalidarg.hpp>
#include <ir2midi/command.hpp>
#include <message/id.hpp>
#include <midi/event.hpp>
#include <midi/limits.hpp>

#include "command_program.hpp"

namespace YAMML
{

namespace IR2MIDI
{

class ProgramCommandProcessor final : public ICommandProcessor, public boost::static_visitor<int>
{
public:
    explicit ProgramCommandProcessor(IIR2MIDICompiler* pCompiler) : m_pCompiler(pCompiler)
    {
    }

    virtual ~ProgramCommandProcessor() = default;

    virtual IIR2MIDICompiler* GetCompiler() override
    {
        return m_pCompiler;
    }

    virtual std::string GetCommandName() const override
    {
        return "program";
    }

    virtual void Process(const AST::Command& ast) override
    {
        ValidateArguments(ast);

        auto channel = boost::get<long>(ast.Arguments[0].Value);

        try
        {
            GetCompiler()->GetTrackContext(channel).PushEvent(0, MIDI::ProgramChange{channel, ast.Arguments[1].Value.apply_visitor(*this)});
        }
        catch (const Exceptions::InvalidArgumentException&)
        {
            ThrowMessage(
                Message::MessageID::InvalidProgram,
                ast.Location,
                {}
            );
        }
    }

    void ValidateArguments(const AST::Command& ast)
    {
        ValidateArgCount(ast, 2);
        ValidateArgType(ast, 0, typeid(long));

        if ((ast.Arguments[1].Value.type() != typeid(long)) && (ast.Arguments[1].Value.type() != typeid(std::string)))
        {
            ThrowMessage(
                Message::MessageID::WrongTypeOfCommandArgument,
                ast.Location,
                {GetCommandName(), "2", "int/string"}
            );
        }

        ValidateArgChannel(ast, 0);
    }

    int operator()(const long& n)
    {
        if ((0 <= n) && (n < 128))
        {
            return static_cast<int>(n);
        }
        else
        {
            throw Exceptions::InvalidArgumentException("IR2MIDI::ProgramCommandProcessor::operator()(const long&)");
        }
    }

    int operator()(const double&)
    {
        throw Exceptions::InvalidArgumentException("IR2MIDI::ProgramCommandProcessor::operator()(const double&)");
    }

    int operator()(const std::string&)
    {
        // TODO
        throw Exceptions::InvalidArgumentException("IR2MIDI::ProgramCommandProcessor::operator()(const std::string&)");
    }

private:
    IIR2MIDICompiler* m_pCompiler;
};

std::unique_ptr<ICommandProcessor> CreateProgramCommandProcessor(IIR2MIDICompiler* pCompiler)
{
    return std::make_unique<ProgramCommandProcessor>(pCompiler);
}

} // namespace IR2MIDI

} // namespace YAMML
