
#include "composition2ir.hpp"

namespace YAMML
{

namespace AST2IR
{

Composition2IRCompiler::Composition2IRCompiler(Compiler::CompilerBase& parentCompiler, IR::Module& ir)
    : NestedCompilerBase(parentCompiler), m_IR(ir)
{
}

bool Composition2IRCompiler::Compile(const AST::Composition& ast, IR::BlockReference index)
{
    return false;
}

} // namespace AST2IR

} // namespace YAMML
