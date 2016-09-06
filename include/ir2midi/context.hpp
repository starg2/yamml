
#pragma once

#include <string>
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
    TrackCompilerContext() = default;

    explicit TrackCompilerContext(int lastEventTime)
    {
        SetLastEventTime(lastEventTime);
    }

    void ResetTime();
    int GetLastEventTime();
    void SetLastEventTime(int t);
    void UpdateTime(int relativeTime);
    void PushEvent(int relativeTime, const MIDI::MIDIEvent::EventType& ev);
    void SortEvents();
    const std::vector<AbsoluteMIDIEvent>& GetEvents() const;

private:
    std::vector<AbsoluteMIDIEvent> m_Events;
    int m_BaseTimeForCurrentBlock = 0;
    int m_LastEventTime = 0;
};

class IIR2MIDICompiler
{
public:
    virtual ~IIR2MIDICompiler() = default;

    virtual std::string GetSourceName() const = 0;
    virtual TrackCompilerContext& GetTrackContext(int trackNumber) = 0;
    virtual bool CompileTrackBlock(const std::string& trackBlockName) = 0;
    virtual bool HasTrackBlock(const std::string& trackBlockName) const = 0;
};

} // namespace IR2MIDI

} // namespace YAMML
