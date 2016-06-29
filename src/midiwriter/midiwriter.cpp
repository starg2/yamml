
#include <cstdint>
#include <vector>

#include <binary/binarywriter.hpp>
#include <exceptions/invalidarg.hpp>
#include <midi/file.hpp>
#include <midiwriter/midiwriter.hpp>

#include "trackeventwriter.hpp"

namespace YAMML
{

namespace MIDIWriter
{

namespace Detail
{

class MIDIWriterImpl final : public Binary::BinaryWriter
{
public:
    void Write(const MIDI::MIDIFile& midi)
    {
        Clear();
        AppendHeader(midi);

        for (auto&& i : midi.Tracks)
        {
            AppendTrack(i);
        }
    }

private:
    void AppendHeader(const MIDI::MIDIFile& midi)
    {
        AppendString("MThd");

        AppendUInt32BE(6);

        AppendUInt16BE(midi.Header.GetFormat());

        if (midi.Tracks.size() > 0xFFFF)
        {
            throw Exceptions::InvalidArgumentException("MIDIWriterImpl::AppendHeader() : too many tracks");
        }

        AppendUInt16BE(static_cast<std::uint16_t>(midi.Tracks.size()));
        AppendUInt16BE(midi.Header.GetUnitsPerBeat());
    }

    void AppendTrack(const MIDI::MIDITrack& track)
    {
        AppendString("MTrk");

        TrackEventWriter trackEventWriter;
        trackEventWriter.Write(track.Events);

        auto events = trackEventWriter.GetBuffer();

        AppendUInt32BE(static_cast<std::uint32_t>(events.size()));
        AppendBuffer(events);
    }
};

} // namespace Detail

std::vector<std::uint8_t> WriteMIDI(const MIDI::MIDIFile& midi)
{
    Detail::MIDIWriterImpl writer;
    writer.Write(midi);
    return writer.GetBuffer();
}

} // namespace MIDIWriter

} // namespace YAMML
