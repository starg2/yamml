
#pragma once

#include <cstdint>
#include <vector>

#include <midi/file.hpp>

namespace YAMML
{

namespace MIDIWriter
{

std::vector<std::uint8_t> WriteMIDI(const MIDI::MIDIFile& midi);

} // namespace MIDIWriter

} // namespace YAMML
