
#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <boost/variant.hpp>

#include <ast/composition.hpp>
#include <compiler/base.hpp>
#include <ir/module.hpp>
#include <ir2midi/command.hpp>
#include <ir2midi/context.hpp>
#include <midi/file.hpp>

namespace YAMML
{

namespace IR2MIDI
{

class IR2MIDICompiler final : public Compiler::CompilerBase, public IIR2MIDICompiler, public boost::static_visitor<>
{
public:
    explicit IR2MIDICompiler(const IR::Module& ir) : m_IR(ir)
    {
        InitializeCommandProcessors();
    }

    template<typename T>
    IR2MIDICompiler(const IR::Module& ir, T func) : CompilerBase(func), m_IR(ir)
    {
        InitializeCommandProcessors();
    }

    IR2MIDICompiler(const IR2MIDICompiler&) = delete;
    IR2MIDICompiler& operator=(const IR2MIDICompiler&) = delete;

    virtual ~IR2MIDICompiler() = default;

    bool Compile(const std::string& entryPoint);

    MIDI::MIDIFile& GetMIDI();
    const MIDI::MIDIFile& GetMIDI() const;

    void operator()(const IR::TrackList& ir);
    void operator()(const AST::Command& ast);

    void operator()(int trackNumber, const IR::Event& ev);
    void operator()(int trackNumber, const IR::BlockReference& blockRef);

    virtual std::string GetSourceName() const override;
    virtual TrackCompilerContext& GetTrackContext(int trackNumber) override;
    virtual bool HasTrackBlock(const std::string& trackBlockName) const override;

private:
    void AddCommandProcessor(std::unique_ptr<ICommandProcessor> pProcessor);
    void InitializeCommandProcessors();
    virtual bool CompileTrackBlock(const std::string& trackBlockName) override;
    void CompileBlock(int trackNumber, IR::BlockReference blockRef);
    void Finalize();

    void CheckForUnprocessedAttributes(const std::vector<AST::Attribute>& attributes);
    void EnsureTrackInitialized(int number);

    int GetLastGlobalEventTime() const;
    void UpdateLastEventTime();
    MIDI::MIDITrack& GetTrack(int trackNumber);

    IR::Module m_IR;
    MIDI::MIDIFile m_MIDI;
    std::vector<TrackCompilerContext> m_Contexts;
    std::unordered_map<std::string, std::unique_ptr<ICommandProcessor>> m_CommandProcessors;
    int m_LastEventTime = 0;
};

} // namespace IR2MIDI

} // namespace YAMML
