
#pragma once

#include <midi/note.hpp>

namespace YAMML
{

namespace IR
{

class Note final
{
public:
    MIDI::NoteNumber Number;
    int OnVelocity;
    int Duration;
    int OffVelocity;
};

} // namespace IR

} // namespace YAMML
