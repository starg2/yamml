
#include <algorithm>
#include <iterator>
#include <string>

#include <exceptions/messageexception.hpp>
#include <message/message.hpp>

#include "composition2ir.hpp"
#include "containerutil.hpp"

namespace YAMML
{

namespace AST2IR
{

constexpr int TrackNumberLimit = 16;

Composition2IRCompiler::Composition2IRCompiler(Compiler::CompilerBase& parentCompiler, IR::Module& ir)
    : NestedCompilerBase(parentCompiler), m_IR(ir)
{
}

bool Composition2IRCompiler::Compile(const AST::Composition& ast, IR::TrackBlockReference index)
{
    try
    {
        m_IR.TrackBlocks.at(index.ID).Attributes = ast.Attributes;

        for (auto&& i : ast.Statements)
        {
            m_IR.TrackBlocks[index.ID].Blocks.push_back(i.apply_visitor(*this));
        }

        return true;
    }
    catch (const Exceptions::MessageException& e)
    {
        AddMessage(e.Item);
        return false;
    }
    catch (const std::exception& e)
    {
        AddMessage(
            Message::MessageItem{
                Message::MessageKind::FetalError,
                Message::MessageID::UnknownInComposition2IR,
                m_IR.Name,
                ast.Location,
                {ast.Name, e.what()}
            }
        );

        return false;
    }
}

IR::TrackBlock::BlockType Composition2IRCompiler::operator()(const AST::Command& ast)
{
    return ast;
}

IR::TrackBlock::BlockType Composition2IRCompiler::operator()(const AST::TrackListBlock& ast)
{
    IR::TrackList trackList;
    trackList.Attributes = ast.Attributes;

    std::transform(
        ast.Tracks.begin(),
        ast.Tracks.end(),
        std::back_inserter(trackList.Tracks),
        [this] (auto&& x) { return this->Compile(x); }
    );

    return trackList;
}

IR::Track Composition2IRCompiler::Compile(const AST::TrackBlock& ast)
{
    if (!(0 <= ast.TrackNumber && ast.TrackNumber < TrackNumberLimit))
    {
        throw Exceptions::MessageException(
            Message::MessageItem{
                Message::MessageKind::Error,
                Message::MessageID::TrackNumberIsOutOfPreferredRange,
                m_IR.Name,
                ast.Location,
                {std::to_string(ast.TrackNumber), std::to_string(TrackNumberLimit)}
            }
        );
    }

    std::vector<IR::TrackItem> items;

    std::transform(
        ast.Items.begin(),
        ast.Items.end(),
        std::back_inserter(items),
        [this] (auto&& x) { return this->Compile(x); }
    );

    return IR::Track{
        ast.Attributes,
        ast.TrackNumber,
        items
    };
}

IR::TrackItem Composition2IRCompiler::Compile(const AST::TrackItem& ast)
{
    auto itName = m_IR.BlockNameMap.find(ast.PhraseName);

    if (itName == m_IR.BlockNameMap.end())
    {
        throw Exceptions::MessageException(
            {
                Message::MessageKind::Error,
                Message::MessageID::NoSuchPhraseName,
                m_IR.Name,
                ast.Location,
                {ast.PhraseName}
            }
        );
    }

    return IR::TrackItem{
        ast.Attributes,
        IR::BlockReference{static_cast<std::size_t>(std::distance(m_IR.BlockNameMap.begin(), itName))}
    };
}

} // namespace AST2IR

} // namespace YAMML
