
#pragma once

#include <deque>
#include <string>
#include <vector>

#include <boost/optional.hpp>
#include <boost/variant.hpp>

#include <ast/attribute.hpp>
#include <ast/phrase.hpp>
#include <compiler/nested.hpp>
#include <ir/block.hpp>
#include <ir/module.hpp>

namespace YAMML
{

namespace AST2IR
{

class Phrase2IRCompiler final : public Compiler::NestedCompilerBase, public boost::static_visitor<std::vector<IR::Block::EventType>>
{
public:
    Phrase2IRCompiler(Compiler::CompilerBase& parentCompiler, IR::Module& ir);

    // Compiles ast into m_IR.Blocks[index.ID]
    bool Compile(const AST::Phrase& ast, IR::BlockReference index);

    std::vector<IR::Block::EventType> operator()(const AST::NoteSequenceStatement& ast);
    std::vector<IR::Block::EventType> operator()(const AST::NoteSequenceBlock& ast);

    std::vector<IR::Block::EventType> operator()(const AST::NoteSequence& ast);
    std::vector<IR::Block::EventType> operator()(const AST::NoteAndExpression& ast);

    std::vector<IR::Block::EventType> operator()(const AST::NoteAndDuration& ast);
    std::vector<IR::Block::EventType> operator()(const AST::NoteRepeatExpression& ast);
    std::vector<IR::Block::EventType> operator()(const AST::NoteRepeatEachExpression& ast);

    std::vector<IR::Block::EventType> operator()(const AST::Rest& ast, int duration);
    std::vector<IR::Block::EventType> operator()(const AST::NoteNumber& ast, int duration);
    std::vector<IR::Block::EventType> operator()(const AST::SimpleChord& ast, int duration);

private:
    int CalculateDuration(const AST::NoteAndDuration& ast);

    IR::BlockReference AllocBlock();
    void Compile(const AST::NoteSequenceBlockWithoutAttributes& ast, IR::BlockReference index);
    void LimitRepeatCount(std::size_t count, const AST::SourceLocation& location);

    IR::Module& m_IR;
    std::deque<std::vector<AST::Attribute>> m_AttributeStack;
    int m_RelativeTime = 0;
};

} // namespace AST2IR

} // namespace YAMML
