
#include <cstdint>

#include <vector>

#include <boost/test/unit_test.hpp>

#include <midi/file.hpp>
#include <midiwriter/midiwriter.hpp>

using namespace YAMML;

BOOST_AUTO_TEST_SUITE(MIDIWriterTest)

BOOST_AUTO_TEST_CASE(Test1)
{
    MIDI::MIDIFile midi;

    midi.Header.SetFormat(1);
    midi.Header.SetUnitsPerBeat(480);

    midi.Tracks.emplace_back();
    midi.Tracks[0].Events.assign({
        MIDI::MIDIEvent{200, MIDI::NoteOff{0, {MIDI::NoteName::C, 5}, 0}},
        MIDI::MIDIEvent{400, MIDI::MetaEvent{MIDI::MetaEventKind::EndOfTrack}}
    });

    std::vector<std::uint8_t> expected{
        0x4D, 0x54, 0x68, 0x64,
        0x00, 0x00, 0x00, 0x06,
        0x00, 0x01,
        0x00, 0x01,
        0x01, 0xE0,

        0x4D, 0x54, 0x72, 0x6B,
        0x00, 0x00, 0x00, 0x0A,

        0x81, 0x48, 0x80, 0x3C, 0x00,
        0x83, 0x10, 0xFF, 0x2F, 0x00
    };

    auto ret = MIDIWriter::WriteMIDI(midi);

    BOOST_CHECK_EQUAL_COLLECTIONS(ret.begin(), ret.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_SUITE_END()
