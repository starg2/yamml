
#pragma once

#include <vector>

#include <midi/header.hpp>
#include <midi/track.hpp>

namespace YAMML
{

namespace MIDI
{

// http://maruyama.breadfish.jp/tech/smf/

class MIDIFile final
{
public:
    MIDIHead Header;
    std::vector<MIDITrack> Tracks;
};

} // namespace MIDI

} // namespace YAMML
