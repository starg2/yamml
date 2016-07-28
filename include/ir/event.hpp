
#pragma once

#include <boost/variant.hpp>

#include <ir/note.hpp>
#include <midi/event.hpp>

namespace YAMML
{

namespace IR
{

class Rest final
{
};

class Event final
{
public:
    int Time;
    boost::variant<
        Note,
        Rest,
        MIDI::PolyphonicAftertouch,
        MIDI::ControlChange,
        MIDI::ProgramChange,
        MIDI::Aftertouch,
        MIDI::PitchBend,
        MIDI::SysExEvent,
        MIDI::MetaEvent
    > Value;
};

} // namespace IR

} // namespace YAMML
