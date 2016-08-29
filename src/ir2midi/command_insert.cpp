
#include <memory>
#include <string>
#include <typeinfo>

#include <boost/variant.hpp>

#include <ir2midi/command.hpp>
#include <message/id.hpp>
#include <midi/event.hpp>

#include "command_insert.hpp"

namespace YAMML
{

namespace IR2MIDI
{

class InsertCommandProcessor final : public ICommandProcessor
{
public:
    explicit InsertCommandProcessor(IIR2MIDICompiler* pCompiler) : m_pCompiler(pCompiler)
    {
    }

    virtual ~InsertCommandProcessor() = default;

    virtual IIR2MIDICompiler* GetCompiler() override
    {
        return m_pCompiler;
    }

    virtual void Process(const AST::Command& ast) override
    {
        ValidateArguments(ast);
        GetCompiler()->CompileTrackBlock(boost::get<std::string>(ast.Arguments[0].Value));
    }

    void ValidateArguments(const AST::Command& ast)
    {
        if (ast.Arguments.size() != 1)
        {
            ThrowMessage(
                Message::MessageID::WrongNumberOfCommandArguments,
                ast.Location,
                {"insert", std::to_string(ast.Arguments.size()), "1"}
            );
        }

        if (ast.Arguments[0].Value.type() != typeid(std::string))
        {
            ThrowMessage(
                Message::MessageID::WrongTypeOfCommandArgument,
                ast.Location,
                {"insert", "1", "string"}
            );
        }

        auto name = boost::get<std::string>(ast.Arguments[0].Value);

        if (!GetCompiler()->HasTrackBlock(name))
        {
            ThrowMessage(
                Message::MessageID::NoSuchCompositionName,
                ast.Location,
                {name}
            );
        }
    }

private:
    IIR2MIDICompiler* m_pCompiler;
};

std::unique_ptr<ICommandProcessor> CreateInsertCommandProcessor(IIR2MIDICompiler* pCompiler)
{
    return std::make_unique<InsertCommandProcessor>(pCompiler);
}

} // namespace IR2MIDI

} // namespace YAMML
