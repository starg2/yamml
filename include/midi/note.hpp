
#pragma once

#include <exceptions/invalidarg.hpp>

namespace YAMML
{

namespace MIDI
{

enum class NoteName
{
    C = 0,
    CSharp = 1,
    DFlat = 1,
    D = 2,
    DSharp = 3,
    EFlat = 3,
    E = 4,
    F = 5,
    FSharp = 6,
    GFlat = 6,
    G = 7,
    GSharp = 8,
    AFlat = 8,
    A = 9,
    ASharp = 10,
    BFlat = 10,
    B = 11
};

class NoteNumber final
{
public:
    NoteNumber() : Name(NoteName::C), Octave(4)
    {
    }

    NoteNumber(NoteName name, int octave) : Name(name), Octave(octave)
    {
    }

    NoteNumber(char name, int minor, int octave)
    {
        int n = (name == 'C') ? 0
            : (name == 'D') ? 2
            : (name == 'E') ? 4
            : (name == 'F') ? 5
            : (name == 'G') ? 7
            : (name == 'A') ? 9
            : (name == 'B') ? 11
            : throw Exceptions::InvalidArgumentException("YAMML::MIDI::NoteNumber::NoteNumber: invalid name");

        Name = static_cast<NoteName>(n + minor);
        Octave = octave;
    }

    int GetMIDINoteNumber() const
    {
        return static_cast<int>(Name) + Octave * 12;
    }

    NoteName Name;
    int Octave;
};

} // namespace MIDI

} // namespace YAMML
