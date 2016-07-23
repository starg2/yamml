
#include <ast2ir/ast2ir.hpp>

#include "module2ir.hpp"

namespace YAMML
{

namespace AST2IR
{

bool AST2IRCompiler::Compile(const AST::Module& mod)
{
    GetIR() = Module2IRCompiler(*this).Compile(mod);
    return !HasErrors();
}

boost::optional<IR::Module>& AST2IRCompiler::GetIR()
{
    return m_IR;
}

const boost::optional<IR::Module>& AST2IRCompiler::GetIR() const
{
    return m_IR;
}

} // namespace AST2IR

} // namespace YAMML

