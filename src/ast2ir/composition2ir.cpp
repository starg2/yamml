
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

bool Composition2IRCompiler::Compile(const AST::Composition& ast, IR::BlockReference index)
{
    try
    {
        m_AttributeStack.push_back(ast.Attributes);
        AutoPop<decltype(m_AttributeStack)> autoPop(m_AttributeStack);

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

} // namespace AST2IR

} // namespace YAMML
