
#pragma once

#include <string>
#include <vector>

#include <boost/variant.hpp>

#include <ast/composition.hpp>
#include <compiler/base.hpp>
#include <ir/module.hpp>
#include <midi/file.hpp>

namespace YAMML
{

namespace IR2MIDI
{

class IR2MIDICompiler : public Compiler::CompilerBase, public boost::static_visitor<>
{
public:
    IR2MIDICompiler(const IR::Module& ir);

    bool Compile(const std::string& entryPoint);

    MIDI::MIDIFile& GetMIDI();
    const MIDI::MIDIFile& GetMIDI() const;

    void operator()(const IR::TrackList& ir);
    void operator()(const AST::Command& ast);

private:
    bool CompileTrackBlock(const std::string& trackBlockName);
    void CheckForUnprocessedAttributes(const std::vector<AST::Attribute>& attributes);
    void EnsureTrackInitialized(int number);

    const IR::Module& m_IR;
    MIDI::MIDIFile m_MIDI;
};

} // namespace IR2MIDI

} // namespace YAMML
