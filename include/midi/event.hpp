
#pragma once

#include <cstdint>
#include <vector>

#include <boost/variant.hpp>

#include <midi/note.hpp>

namespace YAMML
{

namespace MIDI
{

// https://www.midi.org/specifications/item/table-1-summary-of-midi-message

class NoteOff final
{
public:
    int Channel = 0;
    NoteNumber Note = {NoteName::C, 5};
    int Velocity = 0;
};

class NoteOn final
{
public:
    int Channel = 0;
    NoteNumber Note = {NoteName::C, 5};
    int Velocity = 0;
};

class PolyphonicAftertouch final
{
public:
    int Channel = 0;
    NoteNumber Note = {NoteName::C, 5};
    int Pressure = 0;
};

// http://nickfever.com/music/midi-cc-list
enum class ControllerNumber : std::uint8_t
{
    BankSelectMSB = 0,
    Modulation = 1,
    BreathControl = 2,
    FootControl = 4,
    PortamentoTime = 5,
    DataEntryMSB = 6,
    MainVolume = 7,
    BalanceControl = 8,
    Panpot = 10,
    Expression = 11,
    EffectControl1 = 12,
    EffectControl2 = 13,
    GeneralPurpose1 = 16,
    GeneralPurpose2 = 17,
    GeneralPurpose3 = 18,
    GeneralPurpose4 = 19,

    BankSelectLSB = 32,
    DataEntryLSB = 38,

    DamperPedal = 64,
    Portamento = 65,
    Sostenuto = 66,
    SoftPedal = 67,
    Legato = 68,
    Hold2 = 69,
    MemoryPatchSelect = 70,
    Resonance = 71,
    ReleaseTime = 72,
    AttackTime = 73,
    Cutoff = 74,
    DecayTime = 75,
    VibratoRate = 76,
    VibratoDepth = 77,
    VibratoDelay = 78,
    GeneralPurpose5 = 80,
    GeneralPurpose6 = 81,
    GeneralPurpose7 = 82,
    GeneralPurpose8 = 83,
    PortamentoControl = 84,

    Reverb = 91,
    Tremolo = 92,
    Chorus = 93,
    Detune = 94,
    Phaser = 95,

    DataIncrement = 96,
    DataDecrement = 97,
    NrpnLSB = 98,
    NrpnMSB = 99,
    RpnLSB = 100,
    RpnMSB = 101,

    AllSoundOff = 120,
    ResetAllControllers = 121,
    LocalControl = 122,
    AllNotesOff = 123,
    OmniOff = 124,
    OmniOn = 125,
    Mono = 126,
    Poly = 127
};

class ControlChange final
{
public:
    int Channel = 0;
    ControllerNumber Control = ControllerNumber::AllNotesOff;
    int Value = 0;
};

class ProgramChange final
{
public:
    int Channel = 0;
    int Program = 0;
};

class Aftertouch final
{
public:
    int Channel = 0;
    int Pressure = 0;
};

class PitchBend final
{
public:
    int Channel = 0;
    int Value = 0;
};

class SysExEvent final
{
public:
    std::vector<std::uint8_t> Message;
};

enum class MetaEventKind : std::uint8_t
{
    SequenceNumber = 0x0,
    Text = 0x1,
    CopyrightNotice = 0x2,
    SequenceName = 0x3,
    InstrumentName = 0x4,
    Lylic = 0x5,
    EndOfTrack = 0x2F,
    SetTempo = 0x51,
    TimeSignature = 0x58,
    KeySignature = 0x59
};

class MetaEvent final
{
public:
    MetaEventKind Kind;
    std::vector<std::uint8_t> Value;
};

class MIDIEvent final
{
public:
    int DeltaTime;
    boost::variant<
        NoteOff,
        NoteOn,
        PolyphonicAftertouch,
        ControlChange,
        ProgramChange,
        Aftertouch,
        PitchBend,
        SysExEvent,
        MetaEvent
    > Event;
};

} // namespace MIDI

} // namespace YAMML
