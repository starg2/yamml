
#include <exception>

#include <boost/variant.hpp>

#include <exceptions/messageexception.hpp>
#include <ir2midi/ir2midi.hpp>
#include <message/message.hpp>

namespace YAMML
{

namespace IR2MIDI
{

constexpr int TrackNumberSafeLimit = 256;

void TrackCompilerContext::PushEvent(int relativeTime, const MIDI::MIDIEvent::EventType& ev)
{
    m_Events.push_back(AbsoluteMIDIEvent{m_BaseTimeForCurrentBlock + relativeTime, ev});
}

IR2MIDICompiler::IR2MIDICompiler(const IR::Module& ir) : m_IR(ir)
{
}

bool IR2MIDICompiler::Compile(const std::string& entryPoint)
{
    try
    {
        return CompileTrackBlock(entryPoint);
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
                m_IR.Name,
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
        EnsureTrackInitialized(i.Number);

        for (auto&& j : i.Items)
        {
            CheckForUnprocessedAttributes(j.Attributes);
            CompileBlock(i.Number, j.Block);
        }
    }
}

void IR2MIDICompiler::operator()(const AST::Command& ast)
{
    throw Exceptions::MessageException(
        Message::MessageItem{
            Message::MessageKind::FetalError,
            Message::MessageID::UnprocessedCommand,
            m_IR.Name,
            ast.Location,
            {ast.Name}
        }
    );
}

void IR2MIDICompiler::operator()(int trackNumber, const IR::Event& ev)
{

}

void IR2MIDICompiler::operator()(int trackNumber, const IR::BlockReference& blockRef)
{
    CompileBlock(trackNumber, blockRef);
}

bool IR2MIDICompiler::CompileTrackBlock(const std::string& trackBlockName)
{
    auto itTrack = m_IR.TrackBlockNameMap.find(trackBlockName);

    if (itTrack == m_IR.TrackBlockNameMap.end())
    {
        throw Exceptions::MessageException(
            Message::MessageItem{
                Message::MessageKind::Error,
                Message::MessageID::NoSuchCompositionName,
                m_IR.Name,
                {0, 0},
                {trackBlockName}
            }
        );
    }

    // with bounds checking
    CheckForUnprocessedAttributes(m_IR.TrackBlocks.at(itTrack->second.ID).Attributes);

    for (auto&& i : m_IR.TrackBlocks[itTrack->second.ID].Blocks)
    {
        i.apply_visitor(*this);
    }

    return true;
}

void IR2MIDICompiler::CompileBlock(int trackNumber, IR::BlockReference blockRef)
{
    const auto& block = m_IR.Blocks.at(blockRef.ID);
    CheckForUnprocessedAttributes(block.Attributes);

    boost::variant<int> varTrackNumber = trackNumber;

    for (auto&& i : block.Events)
    {
        boost::apply_visitor(*this, varTrackNumber, i);
    }
}

void IR2MIDICompiler::CheckForUnprocessedAttributes(const std::vector<AST::Attribute>& attributes)
{
    if (!attributes.empty())
    {
        throw Exceptions::MessageException(
            Message::MessageItem{
                Message::MessageKind::FetalError,
                Message::MessageID::UnprocessedAttribute,
                m_IR.Name,
                attributes.at(0).Location,
                {attributes.at(0).Name}
            }
        );
    }
}

void IR2MIDICompiler::EnsureTrackInitialized(int number)
{
    if (!(0 <= number && number < TrackNumberSafeLimit))
    {
        throw Exceptions::MessageException(
            Message::MessageItem{
                Message::MessageKind::FetalError,
                Message::MessageID::TrackNumberIsOutOfSafeRange,
                m_IR.Name,
                {0, 0},
                {std::to_string(number), std::to_string(TrackNumberSafeLimit)}
            }
        );
    }

    if (static_cast<std::size_t>(number) >= m_MIDI.Tracks.size())
    {
        m_MIDI.Tracks.resize(number + 1);
        m_Contexts.resize(number + 1);
    }
}

MIDI::MIDITrack& IR2MIDICompiler::GetTrack(int trackNumber)
{
    return m_MIDI.Tracks[static_cast<std::size_t>(trackNumber)];
}

TrackCompilerContext& IR2MIDICompiler::GetTrackContext(int trackNumber)
{
    return m_Contexts[static_cast<std::size_t>(trackNumber)];
}

} // namespace IR2MIDI

} // namespace YAMML
