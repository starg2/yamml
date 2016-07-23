
#include <deque>
#include <string>
#include <vector>

#include <boost/optional.hpp>
#include <boost/variant.hpp>

#include <ast/attribute.hpp>
#include <ast/phrase.hpp>
#include <compiler/nested.hpp>
#include <message/message.hpp>
#include <ir/module.hpp>

#include "containerutil.hpp"
#include "phrase2ir.hpp"

namespace YAMML
{

namespace AST2IR
{

Phrase2IRCompiler::Phrase2IRCompiler(Compiler::CompilerBase& parentCompiler, IR::Module& ir)
    : NestedCompilerBase(parentCompiler), m_IR(ir)
{
}

bool Phrase2IRCompiler::Compile(const AST::Phrase& ast, std::size_t index)
{
    m_AttributeStack.push_back(ast.Attributes);

    AutoPop<decltype(m_AttributeStack)> ap(m_AttributeStack);

    return (*this)(ast.Block);
}

bool Phrase2IRCompiler::operator()(const AST::NoteSequenceBlockWithoutAttributes& ast)
{
    return false;
}

} // namespace AST2IR

} // namespace YAMML
