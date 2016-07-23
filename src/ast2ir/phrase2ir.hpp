
#pragma once

#include <deque>
#include <string>
#include <vector>

#include <boost/optional.hpp>
#include <boost/variant.hpp>

#include <ast/attribute.hpp>
#include <ast/phrase.hpp>
#include <compiler/nested.hpp>
#include <ir/module.hpp>

namespace YAMML
{

namespace AST2IR
{

class Phrase2IRCompiler final : public Compiler::NestedCompilerBase, public boost::static_visitor<bool>
{
public:
    Phrase2IRCompiler(Compiler::CompilerBase& parentCompiler, IR::Module& ir);

    // Compiles ast into m_IR.Blocks[index]
    bool Compile(const AST::Phrase& ast, std::size_t index);

    bool operator()(const AST::NoteSequenceBlockWithoutAttributes& ast);

private:
    IR::Module& m_IR;
    std::deque<std::vector<AST::Attribute>> m_AttributeStack;
};

} // namespace AST2IR

} // namespace YAMML
