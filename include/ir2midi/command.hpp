
#pragma once

#include <string>
#include <vector>

#include <ast/composition.hpp>
#include <exceptions/messageexception.hpp>
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

    virtual void Process(const AST::Command& ast) = 0;

protected:
    virtual IIR2MIDICompiler* GetCompiler() = 0;

    void ThrowMessage(Message::MessageID id, const AST::SourceLocation& location, const std::vector<std::string>& args)
    {
        throw Exceptions::MessageException(
            Message::MessageItem{
                Message::MessageKind::Error,
                id,
                GetCompiler()->GetSourceName(),
                location,
                args
            }
        );
    }
};

} // namespace IR2MIDI

} // namespace YAMML
