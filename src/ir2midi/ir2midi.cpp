
#include <algorithm>
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

class EventConverter final : public boost::static_visitor<>
{
public:
    EventConverter(TrackCompilerContext& context, int relativeTime, int channel)
        : m_Context(context), m_RelativeTime{relativeTime}, m_Channel{channel}
    {
    }

    void operator()(const IR::Note& ev)
    {
        m_Context.PushEvent(m_RelativeTime, MIDI::NoteOn{m_Channel, ev.Number, ev.OnVelocity});
        m_Context.PushEvent(m_RelativeTime + ev.Duration, MIDI::NoteOff{m_Channel, ev.Number, ev.OffVelocity});
    }

    void operator()(const IR::Rest&)
    {
    }

    void operator()(const IR::PolyphonicAftertouch& ev)
    {
        m_Context.PushEvent(m_RelativeTime, MIDI::PolyphonicAftertouch{m_Channel, ev.Note, ev.Pressure});
    }

    void operator()(const IR::ControlChange& ev)
    {
        m_Context.PushEvent(m_RelativeTime, MIDI::ControlChange{m_Channel, ev.Control, ev.Value});
    }

    void operator()(const IR::ProgramChange& ev)
    {
        m_Context.PushEvent(m_RelativeTime, MIDI::ProgramChange{m_Channel, ev.Program});
    }

    void operator()(const IR::Aftertouch& ev)
    {
        m_Context.PushEvent(m_RelativeTime, MIDI::Aftertouch{m_Channel, ev.Pressure});
    }

    void operator()(const IR::PitchBend& ev)
    {
        m_Context.PushEvent(m_RelativeTime, MIDI::PitchBend{m_Channel, ev.Value});
    }

    void operator()(const MIDI::SysExEvent& ev)
    {
        m_Context.PushEvent(m_RelativeTime, ev);
    }

    void operator()(const MIDI::MetaEvent& ev)
    {
        m_Context.PushEvent(m_RelativeTime, ev);
    }

private:
    TrackCompilerContext& m_Context;
    int m_RelativeTime;
    int m_Channel;
};


void TrackCompilerContext::EnterBlock()
{
    m_BaseTimeForCurrentBlock = m_LastEventTime;
}

void TrackCompilerContext::PushEvent(int relativeTime, const MIDI::MIDIEvent::EventType& ev)
{
    m_LastEventTime = m_BaseTimeForCurrentBlock + relativeTime;
    m_Events.push_back(AbsoluteMIDIEvent{m_BaseTimeForCurrentBlock + relativeTime, ev});
}

void TrackCompilerContext::SortEvents()
{
    std::stable_sort(
        m_Events.begin(),
        m_Events.end(),
        [] (auto&& lhs, auto&& rhs)
        {
            return lhs.AbsoluteTime < rhs.AbsoluteTime;
        }
    );
}

const std::vector<AbsoluteMIDIEvent>& TrackCompilerContext::GetEvents() const
{
    return m_Events;
}

bool IR2MIDICompiler::Compile(const std::string& entryPoint)
{
    try
    {
        if (CompileTrackBlock(entryPoint))
        {
            Finalize();
            return true;
        }
        else
        {
            return false;
        }
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
    EventConverter ec(GetTrackContext(trackNumber), ev.Time, trackNumber);
    ev.Value.apply_visitor(ec);
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
    GetTrackContext(trackNumber).EnterBlock();

    const auto& block = m_IR.Blocks.at(blockRef.ID);
    CheckForUnprocessedAttributes(block.Attributes);

    boost::variant<int> varTrackNumber = trackNumber;

    for (auto&& i : block.Events)
    {
        boost::apply_visitor(*this, varTrackNumber, i);
    }
}

void IR2MIDICompiler::Finalize()
{
    for (std::size_t i = 0; i < m_MIDI.Tracks.size(); i++)
    {
        GetTrackContext(i).SortEvents();

        int prevTime = 0;

        for (auto&& j : GetTrackContext(i).GetEvents())
        {
            m_MIDI.Tracks[i].Events.push_back(MIDI::MIDIEvent{j.AbsoluteTime - prevTime, j.Event});
            prevTime = j.AbsoluteTime;
        }

        m_MIDI.Tracks[i].Events.push_back(MIDI::MIDIEvent{prevTime + 10, MIDI::MetaEvent{MIDI::MetaEventKind::EndOfTrack}});
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
