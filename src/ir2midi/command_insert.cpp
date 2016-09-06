
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

    virtual std::string GetCommandName() const override
    {
        return "insert";
    }

    virtual void Process(const AST::Command& ast) override
    {
        ValidateArguments(ast);
        GetCompiler()->CompileTrackBlock(boost::get<std::string>(ast.Arguments[0].Value), ast.Location);
    }

    void ValidateArguments(const AST::Command& ast)
    {
        ValidateArgCount(ast, 1);
        ValidateArgType(ast, 0, typeid(std::string));
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
