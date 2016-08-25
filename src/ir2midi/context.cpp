
#include <algorithm>

#include <ir2midi/context.hpp>

namespace YAMML
{

namespace IR2MIDI
{

void TrackCompilerContext::EnterBlock()
{
    m_BaseTimeForCurrentBlock = m_LastEventTime;
}

int TrackCompilerContext::GetLastEventTime()
{
    return m_LastEventTime;
}

void TrackCompilerContext::SetLastEventTime(int t)
{
    m_LastEventTime = t;
    m_BaseTimeForCurrentBlock = t;
}

void TrackCompilerContext::UpdateTime(int relativeTime)
{
    m_LastEventTime = m_BaseTimeForCurrentBlock + relativeTime;
}

void TrackCompilerContext::PushEvent(int relativeTime, const MIDI::MIDIEvent::EventType& ev)
{
    UpdateTime(relativeTime);
    m_Events.push_back(AbsoluteMIDIEvent{m_LastEventTime, ev});
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

} // namespace IR2MIDI

} // namespace YAMML
