
#pragma once

#include <string>
#include <vector>

#include <ast/composition.hpp>
#include <ir2midi/context.hpp>
#include <message/message.hpp>

namespace YAMML
{

namespace IR2MIDI
{

std::vector<Message::MessageItem> ProcessTempo(IIR2MIDICompiler* pCompiler, const AST::Command& ast);

} // namespace IR2MIDI

} // namespace YAMML
