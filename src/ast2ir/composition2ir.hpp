
#pragma once

#include <deque>
#include <vector>

#include <boost/variant.hpp>

#include <ast/attribute.hpp>
#include <ast/composition.hpp>
#include <compiler/nested.hpp>
#include <ir/module.hpp>
#include <ir/track.hpp>

namespace YAMML
{

namespace AST2IR
{

class Composition2IRCompiler final : public Compiler::NestedCompilerBase, public boost::static_visitor<IR::TrackBlock::BlockType>
{
public:
    Composition2IRCompiler(Compiler::CompilerBase& parentCompiler, IR::Module& ir);

    // Compiles ast into m_IR.Blocks[index.ID]
    bool Compile(const AST::Composition& ast, IR::TrackBlockReference index);

    IR::TrackBlock::BlockType operator()(const AST::Command& ast);
    IR::TrackBlock::BlockType operator()(const AST::TrackListBlock& ast);

private:
    IR::Module& m_IR;
    std::deque<std::vector<AST::Attribute>> m_AttributeStack;
};

} // namespace AST2IR

} // namespace YAMML
