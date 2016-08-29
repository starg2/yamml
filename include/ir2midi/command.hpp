
#pragma once

#include <string>
#include <typeinfo>
#include <vector>

#include <ast/composition.hpp>
#include <ir2midi/context.hpp>
#include <message/message.hpp>

namespace YAMML
{

namespace IR2MIDI
{

class ICommandProcessor
{
public:
    virtual ~ICommandProcessor() = default;

    virtual std::string GetCommandName() const = 0;
    virtual void Process(const AST::Command& ast) = 0;

protected:
    virtual IIR2MIDICompiler* GetCompiler() = 0;

    void ThrowMessage(Message::MessageID id, const AST::SourceLocation& location, const std::vector<std::string>& args);
    void ValidateArgCount(const AST::Command& ast, std::size_t count);
    void ValidateArgType(const AST::Command& ast, std::size_t index, const std::type_info& type);
    void ValidateArgChannel(const AST::Command& ast, std::size_t index);
    void ValidateArgMIDIValue(const AST::Command& ast, std::size_t index, Message::MessageID id);
};

} // namespace IR2MIDI

} // namespace YAMML
