
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
    bool Compile(const IR::Module& ir, const std::string& entryPoint);

    MIDI::MIDIFile& GetMIDI();
    const MIDI::MIDIFile& GetMIDI() const;

    void operator()(const IR::TrackList& ir);
    void operator()(const AST::Command& ast);

private:
    bool CompileTrackBlock(const IR::Module& ir, const std::string& trackBlockName);
    void CheckForUnprocessedAttributes(const std::vector<AST::Attribute>& attributes);

    std::string m_Name;
    MIDI::MIDIFile m_MIDI;
};

} // namespace IR2MIDI

} // namespace YAMML
