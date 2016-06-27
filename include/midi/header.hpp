
#pragma once

#include <cstddef>

#include <exceptions/invalidarg.hpp>

namespace YAMML
{

namespace MIDI
{

class MIDIHead final
{
public:
    MIDIHead() : m_Format{1}, m_UnitsPerBeat{960}
    {
    }
    
    MIDIHead(int format, int unitsPerBeat)
    {
        SetFormat(format);
        SetUnitsPerBeat(unitsPerBeat);
    }
    
    int GetFormat() const
    {
        return m_Format;
    }

    void SetFormat(int format)
    {
        if (format == 0 || format == 1)
        {
            m_Format = format;
        }
        else
        {
            throw Exceptions::InvalidArgumentException("MIDIHead::SetFormat() : invalid format number");
        }
    }
    
    int GetUnitsPerBeat() const
    {
        return m_UnitsPerBeat;
    }

    void SetUnitsPerBeat(int unitsPerBeat)
    {
        if (0 <= unitsPerBeat && unitsPerBeat <= 0x7FFF)
        {
            m_UnitsPerBeat = unitsPerBeat;
        }
        else
        {
            throw Exceptions::InvalidArgumentException("MIDIHead::SetUnitsPerBeat() : unitsPerBeat is out of range");
        }
    }
    
private:
    int m_Format;
    int m_UnitsPerBeat;
};

} // namespace MIDI

} // namespace YAMML
