
#include <exception>

#include <exceptions/messageexception.hpp>
#include <irprocessor/irprocessor.hpp>
#include <message/message.hpp>

namespace YAMML
{

namespace IRProcessor
{

bool IRCompiler::Compile()
{
    try
    {
        std::size_t numberOfTrackBlocks = m_IR.TrackBlocks.size();

        for (std::size_t i = 0; i < numberOfTrackBlocks; i++)
        {
            CompileTrackBlock(i);
        }

        return !HasErrors();
    }
    catch (const Exceptions::MessageException& e)
    {
        AddMessage(e.Item);
        return false;
    }
    catch (const std::exception& e)
    {
        AddMessage(
            Message::MessageItem{
                Message::MessageKind::FetalError,
                Message::MessageID::UnknownInIRProcessor,
                GetSourceName(),
                {0, 0},
                {e.what()}
            }
        );

        return false;
    }
}

std::string IRCompiler::GetSourceName() const
{
    return m_IR.Name;
}

const IR::Module& IRCompiler::GetIR() const
{
    return m_IR;
}

void IRCompiler::CompileTrackBlock(std::size_t index)
{
    // TODO

    if (!m_IR.TrackBlocks[index].Attributes.empty())
    {
        auto attribute = m_IR.TrackBlocks[index].Attributes.at(0);

        throw Exceptions::MessageException(
            Message::MessageItem{
                Message::MessageKind::Error,
                Message::MessageID::InvalidAttributeName,
                GetSourceName(),
                attribute.Location,
                {attribute.Name}
            }
        );
    }
}

} // namespace IRProcessor

} // namespace YAMML
