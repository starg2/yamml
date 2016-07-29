
#pragma once

#include <deque>
#include <vector>

#include <boost/optional.hpp>

#include <ast/attribute.hpp>
#include <ast/composition.hpp>
#include <compiler/nested.hpp>
#include <ir/block.hpp>
#include <ir/module.hpp>

namespace YAMML
{

namespace AST2IR
{

class Composition2IRCompiler final : public Compiler::NestedCompilerBase
{
public:
    Composition2IRCompiler(Compiler::CompilerBase& parentCompiler, IR::Module& ir);

    // Compiles ast into m_IR.Blocks[index.ID]
    bool Compile(const AST::Composition& ast, IR::BlockReference index);

private:
    IR::Module& m_IR;
    std::deque<std::vector<AST::Attribute>> m_AttributeStack;
};

} // namespace AST2IR

} // namespace YAMML
