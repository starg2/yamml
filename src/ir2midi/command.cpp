
#include <string>
#include <typeinfo>
#include <vector>

#include <boost/variant.hpp>

#include <exceptions/invalidarg.hpp>
#include <exceptions/messageexception.hpp>
#include <ir2midi/command.hpp>
#include <ir2midi/context.hpp>
#include <message/message.hpp>
#include <midi/limits.hpp>

namespace YAMML
{

namespace IR2MIDI
{

std::string GetTypeName(const std::type_info& type)
{
    if (type == typeid(long))
    {
        return "int";
    }
    else if (type == typeid(double))
    {
        return "double";
    }
    else if (type == typeid(std::string))
    {
        return "string";
    }
    else
    {
        throw Exceptions::InvalidArgumentException("YAMML::IR2MIDI::GetTypeName(): Unknown type.");
    }
}

void ICommandProcessor::ThrowMessage(Message::MessageID id, const AST::SourceLocation& location, const std::vector<std::string>& args)
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

void ICommandProcessor::ValidateArgCount(const AST::Command & ast, std::size_t count)
{
    if (ast.Arguments.size() != count)
    {
        ThrowMessage(
            Message::MessageID::WrongNumberOfCommandArguments,
            ast.Location,
            {GetCommandName(), std::to_string(ast.Arguments.size()), std::to_string(count)}
        );
    }
}

void ICommandProcessor::ValidateArgType(const AST::Command& ast, std::size_t index, const std::type_info& type)
{
    if (ast.Arguments[index].Value.type() != type)
    {
        ThrowMessage(
            Message::MessageID::WrongTypeOfCommandArgument,
            ast.Location,
            {GetCommandName(), std::to_string(index + 1), GetTypeName(type)}
        );
    }
}

void ICommandProcessor::ValidateArgChannel(const AST::Command& ast, std::size_t index)
{
    auto channel = boost::get<long>(ast.Arguments[index].Value);

    if (!(0 <= channel && channel < MIDI::TrackNumberLimit))
    {
        ThrowMessage(
            Message::MessageID::TrackNumberIsOutOfPreferredRange,
            ast.Location,
            {std::to_string(channel), std::to_string(MIDI::TrackNumberLimit)}
        );
    }
}

void ICommandProcessor::ValidateArgMIDIValue(const AST::Command& ast, std::size_t index, Message::MessageID id)
{
    auto value = boost::get<long>(ast.Arguments[index].Value);

    if (!(0 <= value && value < 128))
    {
        ThrowMessage(
            id,
            ast.Location,
            {std::to_string(value)}
        );
    }
}

} // namespace IR2MIDI

} // namespace YAMML
