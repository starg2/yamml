
#pragma once

#include <string>
#include <vector>

#include <boost/variant.hpp>

#include <ast/composition.hpp>
#include <compiler/base.hpp>
#include <ir/module.hpp>
#include <midi/event.hpp>
#include <midi/file.hpp>

namespace YAMML
{

namespace IR2MIDI
{

class AbsoluteMIDIEvent final
{
public:
    int AbsoluteTime;
    MIDI::MIDIEvent::EventType Event;
};

class TrackCompilerContext final
{
public:
    void EnterBlock();
    void PushEvent(int relativeTime, const MIDI::MIDIEvent::EventType& ev);
    void SortEvents();
    const std::vector<AbsoluteMIDIEvent>& GetEvents() const;

private:
    std::vector<AbsoluteMIDIEvent> m_Events;
    int m_BaseTimeForCurrentBlock = 0;
    int m_LastEventTime = 0;
};

class IR2MIDICompiler : public Compiler::CompilerBase, public boost::static_visitor<>
{
public:
    IR2MIDICompiler(const IR::Module& ir);

    bool Compile(const std::string& entryPoint);

    MIDI::MIDIFile& GetMIDI();
    const MIDI::MIDIFile& GetMIDI() const;

    void operator()(const IR::TrackList& ir);
    void operator()(const AST::Command& ast);

    void operator()(int trackNumber, const IR::Event& ev);
    void operator()(int trackNumber, const IR::BlockReference& blockRef);

private:
    bool CompileTrackBlock(const std::string& trackBlockName);
    void CompileBlock(int trackNumber, IR::BlockReference blockRef);
    void Finalize();

    void CheckForUnprocessedAttributes(const std::vector<AST::Attribute>& attributes);
    void EnsureTrackInitialized(int number);

    MIDI::MIDITrack& GetTrack(int trackNumber);
    TrackCompilerContext& GetTrackContext(int trackNumber);

    const IR::Module& m_IR;
    MIDI::MIDIFile m_MIDI;
    std::vector<TrackCompilerContext> m_Contexts;
};

} // namespace IR2MIDI

} // namespace YAMML
