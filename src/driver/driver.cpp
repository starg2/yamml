
#include <exception>
#include "driver.hpp"

#include <ast2ir/ast2ir.hpp>
#include <ir2midi/ir2midi.hpp>
#include <midiwriter/midiwriter.hpp>
#include <parser/parser.hpp>

namespace YAMML
{

namespace Driver
{

boost::optional<std::vector<std::uint8_t>> CompileYAMML(
    const std::string& sourceName,
    const std::string& source,
    const std::string& entryPoint,
    std::function<bool(const Message::MessageItem&)> callback
)
{
    try
    {
        Parser::YAMMLParser parser(sourceName, source, callback);

        if (!parser.Parse())
        {
            return {};
        }

        AST2IR::AST2IRCompiler ast2ir(callback);

        if (!ast2ir.Compile(parser.GetAST().value()))
        {
            return {};
        }

        IR2MIDI::IR2MIDICompiler ir2midi(ast2ir.GetIR().value(), callback);

        if (!ir2midi.Compile(entryPoint))
        {
            return {};
        }

        return MIDIWriter::WriteMIDI(ir2midi.GetMIDI());
    }
    catch (const std::exception& e)
    {
        callback(
            Message::MessageItem{
                Message::MessageKind::FetalError,
                Message::MessageID::Unknown,
                sourceName,
                {0, 0},
                {e.what()}
            }
        );

        return {};
    }
}

} // namespace Driver

} // namespace YAMML