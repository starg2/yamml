
#include <message/message.hpp>

#include "composition2ir.hpp"
#include "containerutil.hpp"

namespace YAMML
{

namespace AST2IR
{

Composition2IRCompiler::Composition2IRCompiler(Compiler::CompilerBase& parentCompiler, IR::Module& ir)
    : NestedCompilerBase(parentCompiler), m_IR(ir)
{
}

bool Composition2IRCompiler::Compile(const AST::Composition& ast, IR::TrackBlockReference index)
{
    try
    {
        m_AttributeStack.push_back(ast.Attributes);
        AutoPop<decltype(m_AttributeStack)> autoPop(m_AttributeStack);
        m_IR.TrackBlocks.at(index.ID).Attributes = m_AttributeStack.back();

        for (auto&& i : ast.Statements)
        {
            m_IR.TrackBlocks[index.ID].Blocks.push_back(i.apply_visitor(*this));
        }

        return true;
    }
    catch (const std::exception& e)
    {
        AddMessage(
        {
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
    trackList.Tracks.reserve(ast.Tracks.size());

    for (auto&& i : ast.Tracks)
    {

    }

    return trackList;
}

} // namespace AST2IR

} // namespace YAMML
