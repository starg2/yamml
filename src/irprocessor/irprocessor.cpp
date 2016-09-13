
#include <algorithm>
#include <exception>
#include <utility>

#include <common/containerutil.hpp>
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

void IRCompiler::InitializeAttributeProcessorFactories()
{

}

void IRCompiler::AddAttributeProcessorFactory(std::unique_ptr<IAttributeProcessorFactory> pAttributeProcessorFactory)
{
    auto name = pAttributeProcessorFactory->GetAttributeName();
    m_AttributeProcessorFactoryMap[name] = std::move(pAttributeProcessorFactory);
}

void IRCompiler::RegisterAttributeProcessors(const std::vector<AST::Attribute>& attributes)
{
    std::vector<std::unique_ptr<IAttributeProcessor>> attrProcessors;
    attrProcessors.reserve(attributes.size());

    for (auto&& i : attributes)
    {
        auto itFactory = m_AttributeProcessorFactoryMap.find(i.Name);

        if (itFactory == m_AttributeProcessorFactoryMap.end())
        {
            throw Exceptions::MessageException(
                Message::MessageItem{
                    Message::MessageKind::Error,
                    Message::MessageID::InvalidAttributeName,
                    GetSourceName(),
                    i.Location,
                    {i.Name}
                }
            );
        }
        else
        {
            attrProcessors.push_back(itFactory->second->CreateProcessor(i));
        }
    }

    m_AttributeProcessorStack.push_back(std::move(attrProcessors));
}

IR::BlockReference IRCompiler::DuplicateBlock(IR::BlockReference blockRef)
{
    IR::BlockReference newRef{m_IR.Blocks.size()};
    m_IR.Blocks.push_back(m_IR.Blocks.at(blockRef.ID));
    return newRef;
}

void IRCompiler::CompileTrackBlock(std::size_t index)
{
    RegisterAttributeProcessors(m_IR.TrackBlocks.at(index).Attributes);
    Common::AutoPop<decltype(m_AttributeProcessorStack)> autoPop(m_AttributeProcessorStack);

    for (auto&& i : m_IR.TrackBlocks[index].Blocks)
    {
        i.apply_visitor(*this);
    }
}

void IRCompiler::operator()(IR::TrackList& trackList)
{
    RegisterAttributeProcessors(trackList.Attributes);
    Common::AutoPop<decltype(m_AttributeProcessorStack)> autoPop(m_AttributeProcessorStack);

    for (auto&& i : trackList.Tracks)
    {
        RegisterAttributeProcessors(i.Attributes);
        Common::AutoPop<decltype(m_AttributeProcessorStack)> autoPop2(m_AttributeProcessorStack);

        for (auto&& j : i.Items)
        {
            RegisterAttributeProcessors(j.Attributes);
            Common::AutoPop<decltype(m_AttributeProcessorStack)> autoPop3(m_AttributeProcessorStack);

            auto dupRef = DuplicateBlock(j.Block);
            j.Block = dupRef;

            CompileBlock(dupRef);
        }
    }
}

void IRCompiler::operator()(AST::Command&)
{
    // no-op
}

void IRCompiler::CompileBlock(IR::BlockReference blockRef)
{
    RegisterAttributeProcessors(m_IR.Blocks.at(blockRef.ID).Attributes);
    Common::AutoPop<decltype(m_AttributeProcessorStack)> autoPop(m_AttributeProcessorStack);

    for (auto&& i : m_IR.Blocks[blockRef.ID].Events)
    {
        i.apply_visitor(*this);
    }
}

void IRCompiler::operator()(IR::Event& ev)
{
    std::for_each(
        m_AttributeProcessorStack.rbegin(),
        m_AttributeProcessorStack.rend(),
        [&ev] (auto&& x)
        {
            for (auto&& i : x)
            {
                i->TransformEvent(ev);
            }
        }
    );
}

void IRCompiler::operator()(IR::BlockReference& blockRef)
{
    // 'blockRef' here should be referenced by a single block, thus no need to duplicate it.
    // blockRef = DuplicateBlock(blockRef);
    CompileBlock(blockRef);
}

} // namespace IRProcessor

} // namespace YAMML
