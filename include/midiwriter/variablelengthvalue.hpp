
#pragma once

#include <cstdint>
#include <vector>

namespace YAMML
{

namespace MIDIWriter
{

std::vector<std::uint8_t> ToVariableLengthValue(std::uint32_t n);

} // namespace MIDIWriter

} // namespace YAMML
