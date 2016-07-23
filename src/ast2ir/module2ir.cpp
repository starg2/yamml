
#include <message/message.hpp>

#include "module2ir.hpp"

#include "composition2ir.hpp"
#include "phrase2ir.hpp"

namespace YAMML
{

namespace AST2IR
{

template<typename TCompiler, Message::MessageID TMessageID, typename TParentCompiler, typename TItem>
bool ProcessItem(TParentCompiler& parentCompiler, IR::Module& ir, const TItem& item)
{
    auto itName = ir.BlockNameMap.find(item.Name);

    if (itName != ir.BlockNameMap.end())
    {
        parentCompiler.AddMessage(
            {
                Message::MessageKind::Error,
                TMessageID,
                ir.Name,
                item.Location,
                {item.Name}
            }
        );

        return false;
    }

    auto newIndex = ir.Blocks.size();
    ir.BlockNameMap[item.Name] = newIndex;

    return TCompiler(parentCompiler, ir).Compile(item, newIndex);
}

boost::optional<IR::Module> Module2IRCompiler::Compile(const AST::Module& ast)
{
    IR::Module ir;
    ir.Name = ast.Name;

    for (auto&& i : ast.Phrases)
    {
        if (!ProcessItem<Phrase2IRCompiler, Message::MessageID::DuplicatedPhraseName>(*this, ir, i))
        {
            return {};
        }
    }

    for (auto&& i : ast.Compositions)
    {
        if (!ProcessItem<Composition2IRCompiler, Message::MessageID::DuplicatedCompositionName>(*this, ir, i))
        {
            return {};
        }
    }

    return ir;
}

} // namespace AST2IR

} // namespace YAMML
