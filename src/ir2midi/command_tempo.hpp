
#pragma once

#include <memory>

#include <ir2midi/command.hpp>
#include <ir2midi/context.hpp>

namespace YAMML
{

namespace IR2MIDI
{

std::unique_ptr<ICommandProcessor> CreateTempoCommandProcessor(IIR2MIDICompiler* pCompiler);

} // namespace IR2MIDI

} // namespace YAMML
