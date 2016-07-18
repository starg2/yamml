
#include <ast2ir/ast2ir.hpp>


namespace YAMML
{

namespace AST2IR
{

bool AST2IRCompiler::Compile(const AST::Module& mod)
{
    return false;
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

