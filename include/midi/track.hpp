
#pragma once

#include <vector>

#include <midi/event.hpp>

namespace YAMML
{

namespace MIDI
{

class MIDITrack final
{
public:
    std::vector<MIDIEvent> Events;
};

} // namespace MIDI

} // namespace YAMML
