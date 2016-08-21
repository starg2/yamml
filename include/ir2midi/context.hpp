
#pragma once

#include <vector>

#include <midi/event.hpp>

namespace YAMML
{

namespace IR2MIDI
{

class AbsoluteMIDIEvent final
{
public:
    int AbsoluteTime;
    MIDI::MIDIEvent::EventType Event;
};

class TrackCompilerContext final
{
public:
    void EnterBlock();
    void PushEvent(int relativeTime, const MIDI::MIDIEvent::EventType& ev);
    void SortEvents();
    const std::vector<AbsoluteMIDIEvent>& GetEvents() const;

private:
    std::vector<AbsoluteMIDIEvent> m_Events;
    int m_BaseTimeForCurrentBlock = 0;
    int m_LastEventTime = 0;
};

} // namespace IR2MIDI

} // namespace YAMML
