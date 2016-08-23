
#pragma once

#include <boost/variant.hpp>

#include <ir/note.hpp>
#include <midi/event.hpp>
#include <midi/note.hpp>

namespace YAMML
{

namespace IR
{

class Rest final
{
public:
    int Duration;
};

class PolyphonicAftertouch final
{
public:
    MIDI::NoteNumber Note;
    int Pressure;
};

class ControlChange final
{
public:
    MIDI::ControllerNumber Control;
    int Value;
};

class ProgramChange final
{
public:
    int Program;
};

class Aftertouch final
{
public:
    int Pressure;
};

class PitchBend final
{
public:
    int Value;
};

class Event final
{
public:
    using EventType = boost::variant<
        Note,
        Rest,
        PolyphonicAftertouch,
        ControlChange,
        ProgramChange,
        Aftertouch,
        PitchBend,
        MIDI::SysExEvent,
        MIDI::MetaEvent
    >;

    int Time;
    EventType Value;
};

} // namespace IR

} // namespace YAMML
