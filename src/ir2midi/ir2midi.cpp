
#include <exception>

#include <exceptions/messageexception.hpp>
#include <ir2midi/ir2midi.hpp>
#include <message/message.hpp>

namespace YAMML
{

namespace IR2MIDI
{

bool IR2MIDICompiler::Compile(const IR::Module& ir, const std::string& entryPoint)
{
    try
    {
        m_Name = ir.Name;
        return CompileTrackBlock(ir, entryPoint);
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
                Message::MessageID::UnknownInIR2MIDI,
                ir.Name,
                {0, 0},
                {e.what()}
            }
        );

        return false;
    }
}

MIDI::MIDIFile& IR2MIDICompiler::GetMIDI()
{
    return m_MIDI;
}

const MIDI::MIDIFile& IR2MIDICompiler::GetMIDI() const
{
    return m_MIDI;
}

void IR2MIDICompiler::operator()(const IR::TrackList& ir)
{
    CheckForUnprocessedAttributes(ir.Attributes);

    for (auto&& i : ir.Tracks)
    {
        CheckForUnprocessedAttributes(i.Attributes);


    }
}

void IR2MIDICompiler::operator()(const AST::Command& ast)
{
    throw Exceptions::MessageException(
        Message::MessageItem{
            Message::MessageKind::FetalError,
            Message::MessageID::UnprocessedCommand,
            m_Name,
            ast.Location,
            {ast.Name}
        }
    );
}

bool IR2MIDICompiler::CompileTrackBlock(const IR::Module& ir, const std::string& trackBlockName)
{
    auto itTrack = ir.TrackBlockNameMap.find(trackBlockName);

    if (itTrack == ir.TrackBlockNameMap.end())
    {
        throw Exceptions::MessageException(
            Message::MessageItem{
                Message::MessageKind::Error,
                Message::MessageID::NoSuchCompositionName,
                ir.Name,
                {0, 0},
                {trackBlockName}
            }
        );
    }

    // with bounds checking
    CheckForUnprocessedAttributes(ir.TrackBlocks.at(itTrack->second.ID).Attributes);

    for (auto&& i : ir.TrackBlocks[itTrack->second.ID].Blocks)
    {
        i.apply_visitor(*this);
    }

    return true;
}

void IR2MIDICompiler::CheckForUnprocessedAttributes(const std::vector<AST::Attribute>& attributes)
{
    if (!attributes.empty())
    {
        throw Exceptions::MessageException(
            Message::MessageItem{
                Message::MessageKind::FetalError,
                Message::MessageID::UnprocessedAttribute,
                m_Name,
                attributes.at(0).Location,
                {attributes.at(0).Name}
            }
        );
    }
}

} // namespace IR2MIDI

} // namespace YAMML
