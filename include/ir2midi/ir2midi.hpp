
#pragma once

#include <string>

#include <compiler/base.hpp>
#include <ir/module.hpp>
#include <midi/file.hpp>

namespace YAMML
{

namespace IR2MIDI
{

class IR2MIDICompiler : public Compiler::CompilerBase
{
public:
    bool Compile(const IR::Module& ir, const std::string& entryPoint);

    MIDI::MIDIFile& GetMIDI();
    const MIDI::MIDIFile& GetMIDI() const;

private:
    bool CompileTrackBlock(const IR::Module& ir, const std::string& trackBlock);

    MIDI::MIDIFile m_MIDI;
};

} // namespace IR2MIDI

} // namespace YAMML
