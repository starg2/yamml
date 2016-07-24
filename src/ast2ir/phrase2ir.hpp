
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

class Phrase2IRCompiler final : public Compiler::NestedCompilerBase, public boost::static_visitor<IR::BlockReference>
{
public:
    Phrase2IRCompiler(Compiler::CompilerBase& parentCompiler, IR::Module& ir);

    // Compiles ast into m_IR.Blocks[index.ID]
    bool Compile(const AST::Phrase& ast, IR::BlockReference index);

    IR::BlockReference operator()(const AST::NoteSequenceStatement& ast);
    IR::BlockReference operator()(const AST::NoteSequenceBlock& ast);

    IR::BlockReference operator()(const AST::NoteSequence& ast);

    IR::BlockReference operator()(const AST::NoteAndDuration& ast);
    IR::BlockReference operator()(const AST::NoteRepeatExpression& ast);
    IR::BlockReference operator()(const AST::NoteRepeatEachExpression& ast);

private:
    void Compile(const AST::NoteSequenceBlockWithoutAttributes& ast, IR::BlockReference index);
    void LimitRepeatCount(std::size_t count, const AST::SourceLocation& location);

    IR::Module& m_IR;
    std::deque<std::vector<AST::Attribute>> m_AttributeStack;
};

} // namespace AST2IR

} // namespace YAMML
