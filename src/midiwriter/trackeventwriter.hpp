
#pragma once

#include <cstdint>
#include <algorithm>
#include <iterator>
#include <vector>

#include <boost/variant.hpp>

#include <binary/binarywriter.hpp>
#include <exceptions/invalidarg.hpp>
#include <midi/track.hpp>
#include <midiwriter/variablelengthvalue.hpp>

namespace YAMML
{

namespace MIDIWriter
{

namespace Detail
{

class TrackEventWriter final : public Binary::BinaryWriter, public boost::static_visitor<std::vector<std::uint8_t>>
{
public:
    void Write(const std::vector<MIDI::MIDIEvent>& events)
    {
        for (auto&& i : events)
        {
            Write(i);
        }
    }

    void Write(const MIDI::MIDIEvent& ev)
    {
        AppendBuffer(ToVariableLengthValue(ev.DeltaTime));
        AppendBuffer(boost::apply_visitor(*this, ev.Event));
    }

    std::vector<std::uint8_t> operator()(const MIDI::NoteOff& ev) const
    {
        return PackBytes(0x80, ev.Channel, ev.Note.GetMIDINoteNumber(), ev.Velocity);
    }

    std::vector<std::uint8_t> operator()(const MIDI::NoteOn& ev) const
    {
        return PackBytes(0x90, ev.Channel, ev.Note.GetMIDINoteNumber(), ev.Velocity);
    }

    std::vector<std::uint8_t> operator()(const MIDI::PolyphonicAftertouch& ev) const
    {
        return PackBytes(0xA0, ev.Channel, ev.Note.GetMIDINoteNumber(), ev.Pressure);
    }

    std::vector<std::uint8_t> operator()(const MIDI::ControlChange& ev) const
    {
        return PackBytes(0xB0, ev.Channel, ev.Control, ev.Value);
    }

    std::vector<std::uint8_t> operator()(const MIDI::ProgramChange& ev) const
    {
        return PackBytes(0xC0, ev.Channel, ev.Program);
    }

    std::vector<std::uint8_t> operator()(const MIDI::Aftertouch& ev) const
    {
        return PackBytes(0xD0, ev.Channel, ev.Pressure);
    }

    std::vector<std::uint8_t> operator()(const MIDI::PitchBend& ev) const
    {
        return PackBytes(0xE0, ev.Channel, ev.Value & 0x7F, ev.Value >> 7);
    }

    std::vector<std::uint8_t> operator()(const MIDI::SysExEvent& ev) const
    {
        std::vector<std::uint8_t> ret{0xF0};

        auto length = ToVariableLengthValue(static_cast<std::uint32_t>(ev.Message.size()));
        std::copy(length.begin(), length.end(), std::back_inserter(ret));

        std::copy(ev.Message.begin(), ev.Message.end(), std::back_inserter(ret));

        ret.push_back(0xF7);

        return ret;
    }

    std::vector<std::uint8_t> operator()(const MIDI::MetaEvent& ev) const
    {
		std::vector<std::uint8_t> ret{0xFF, ToUInt8(static_cast<std::uint8_t>(ev.Kind), 127)};

        auto length = ToVariableLengthValue(static_cast<std::uint32_t>(ev.Value.size()));
        std::copy(length.begin(), length.end(), std::back_inserter(ret));

        std::copy(ev.Value.begin(), ev.Value.end(), std::back_inserter(ret));

        return ret;
    }

private:
    template<typename... TArgs>
    static std::vector<std::uint8_t> PackBytes(int flag, int channel, TArgs&&... args)
    {
		return {static_cast<std::uint8_t>(ToUInt8(flag, 0xFF) | ToUInt8(channel, 15)), ToUInt8(static_cast<int>(args), 0x7F)...};
    }

    template<typename T>
    static std::uint8_t ToUInt8(T&& x, std::uint8_t max)
    {
        if (0 <= x && x <= max)
        {
            return static_cast<std::uint8_t>(x);
        }
        else
        {
            throw Exceptions::InvalidArgumentException("TrackEventWriter::ToUInt8() : out of range");
        }
    }
};

} // namespace Detail

} // namespace MIDIWriter

} // namespace YAMML
