
#include <cstdint>
#include <vector>

#include <exceptions/invalidarg.hpp>
#include <midiwriter/variablelengthvalue.hpp>

namespace YAMML
{

namespace MIDIWriter
{

std::vector<std::uint8_t> ToVariableLengthValue(std::uint32_t n)
{
    if (n >= (1 << 28))
    {
        throw Exceptions::InvalidArgumentException("ToVariableLengthValue() : n is too large");
    }

    std::vector<std::uint8_t> bytes{static_cast<std::uint8_t>(n & 0x7F)};

    for (std::uint32_t i = n >> 7; i > 0; i >>= 7)
    {
        bytes.insert(bytes.begin(), static_cast<std::uint8_t>(0x80 | (i & 0x7F)));
    }

    return bytes;
}

} // namespace MIDIWriter

} // namespace YAMML
