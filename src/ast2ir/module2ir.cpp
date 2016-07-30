
#include <ir/block.hpp>
#include <ir/track.hpp>
#include <message/message.hpp>

#include "module2ir.hpp"

#include "composition2ir.hpp"
#include "phrase2ir.hpp"

namespace YAMML
{

namespace AST2IR
{

boost::optional<IR::Module> Module2IRCompiler::Compile(const AST::Module& ast)
{
    IR::Module ir;
    ir.Name = ast.Name;

    for (auto&& i : ast.Phrases)
    {
        auto itName = ir.BlockNameMap.find(i.Name);

        if (itName != ir.BlockNameMap.end())
        {
            AddMessage(
            {
                Message::MessageKind::Error,
                Message::MessageID::DuplicatedPhraseName,
                ir.Name,
                i.Location,
                {i.Name}
            }
            );

            return {};
        }

        auto newIndex = IR::BlockReference{ir.Blocks.size()};
        ir.BlockNameMap[i.Name] = newIndex;
        ir.Blocks.emplace_back();

        if (!Phrase2IRCompiler(*this, ir).Compile(i, newIndex))
        {
            return {};
        }
    }

    for (auto&& i : ast.Compositions)
    {
        auto itName = ir.TrackBlockNameMap.find(i.Name);

        if (itName != ir.TrackBlockNameMap.end())
        {
            AddMessage(
            {
                Message::MessageKind::Error,
                Message::MessageID::DuplicatedCompositionName,
                ir.Name,
                i.Location,
                {i.Name}
            }
            );

            return {};
        }

        auto newIndex = IR::TrackBlockReference{ir.TrackBlocks.size()};
        ir.TrackBlockNameMap[i.Name] = newIndex;
        ir.TrackBlocks.emplace_back();

        if (!Composition2IRCompiler(*this, ir).Compile(i, newIndex))
        {
            return {};
        }
    }

    return ir;
}

} // namespace AST2IR

} // namespace YAMML
