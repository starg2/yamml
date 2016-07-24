
#include <cassert>

#include <algorithm>
#include <deque>
#include <exception>
#include <string>
#include <vector>

#include <boost/optional.hpp>
#include <boost/variant.hpp>

#include <ast/attribute.hpp>
#include <ast/phrase.hpp>
#include <compiler/nested.hpp>
#include <exceptions/messageexception.hpp>
#include <message/message.hpp>
#include <ir/block.hpp>
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

bool Phrase2IRCompiler::Compile(const AST::Phrase& ast, IR::BlockReference index)
{
    try
    {
        m_AttributeStack.push_back(ast.Attributes);
        AutoPop<decltype(m_AttributeStack)> autoPop(m_AttributeStack);

        Compile(ast.Block, index);
        return true;
    }
    catch (const std::exception& e)
    {
        AddMessage(
        {
            Message::MessageKind::FetalError,
            Message::MessageID::UnknownInPhrase2IR,
            m_IR.Name,
            ast.Location,
            {ast.Name, e.what()}
        }
        );

        return false;
    }
}

IR::BlockReference Phrase2IRCompiler::operator()(const AST::NoteSequenceStatement& ast)
{
    auto newIndex = IR::BlockReference{m_IR.Blocks.size()};
    m_IR.Blocks.emplace_back();

    {
        m_AttributeStack.push_back(ast.Attributes);
        AutoPop<decltype(m_AttributeStack)> autoPop(m_AttributeStack);

        // with bounds checking
        m_IR.Blocks.at(newIndex.ID).Attributes = Concat(m_AttributeStack);

        if (ast.NoteSeq.is_initialized())
        {
            m_IR.Blocks[newIndex.ID].Events.emplace_back((*this)(*ast.NoteSeq));
        }
    }

    return newIndex;
}

IR::BlockReference Phrase2IRCompiler::operator()(const AST::NoteSequenceBlock& ast)
{
    auto newIndex = IR::BlockReference{m_IR.Blocks.size()};
    m_IR.Blocks.emplace_back();

    m_AttributeStack.push_back(ast.Attributes);
    AutoPop<decltype(m_AttributeStack)> autoPop(m_AttributeStack);

    Compile(ast.Block, newIndex);
    return newIndex;
}

IR::BlockReference Phrase2IRCompiler::operator()(const AST::NoteSequence& ast)
{
    auto newIndex = IR::BlockReference{m_IR.Blocks.size()};
    m_IR.Blocks.emplace_back();

    // with bounds checking
    m_IR.Blocks.at(newIndex.ID).Attributes = Concat(m_AttributeStack);

    for (auto&& i : ast.Notes)
    {
        for (auto&& j : i.Notes)
        {
            m_IR.Blocks[newIndex.ID].Events.emplace_back(j.apply_visitor(*this));
        }
    }

    return newIndex;
}

IR::BlockReference Phrase2IRCompiler::operator()(const AST::NoteAndDuration& ast)
{
    return {};
}

IR::BlockReference Phrase2IRCompiler::operator()(const AST::NoteRepeatExpression& ast)
{
    auto newIndex = IR::BlockReference{m_IR.Blocks.size()};
    m_IR.Blocks.emplace_back();

    // with bounds checking
    m_IR.Blocks.at(newIndex.ID).Attributes = Concat(m_AttributeStack);

    LimitRepeatCount(ast.Count, ast.Location);

    std::vector<IR::BlockReference> childBlockRefArray(ast.Notes.size());
    std::transform(
        ast.Notes.begin(),
        ast.Notes.end(),
        childBlockRefArray.begin(),
        [this] (auto&& x) { return x.apply_visitor(*this); }
    );

    for (std::size_t i = 0; i < ast.Count; i++)
    {
        auto& events = m_IR.Blocks[newIndex.ID].Events;
        events.insert(events.end(), childBlockRefArray.begin(), childBlockRefArray.end());
    }

    return newIndex;
}

IR::BlockReference Phrase2IRCompiler::operator()(const AST::NoteRepeatEachExpression& ast)
{
    auto newIndex = IR::BlockReference{m_IR.Blocks.size()};
    m_IR.Blocks.emplace_back();

    // with bounds checking
    m_IR.Blocks.at(newIndex.ID).Attributes = Concat(m_AttributeStack);

    LimitRepeatCount(ast.Count, ast.Location);

    for (auto&& i : ast.Notes)
    {
        auto childBlockRef = i.apply_visitor(*this);

        for (std::size_t j = 0; j < ast.Count; j++)
        {
            m_IR.Blocks[newIndex.ID].Events.emplace_back(childBlockRef);
        }
    }

    return newIndex;
}

void Phrase2IRCompiler::Compile(const AST::NoteSequenceBlockWithoutAttributes& ast, IR::BlockReference index)
{
    // with bounds checking
    m_IR.Blocks.at(index.ID).Attributes = Concat(m_AttributeStack);

    for (auto&& i : ast.Sequences)
    {
        m_IR.Blocks[index.ID].Events.emplace_back(i.apply_visitor(*this));
    }
}

void Phrase2IRCompiler::LimitRepeatCount(std::size_t count, const AST::SourceLocation& location)
{
    constexpr std::size_t MaxRepeatCount = 1024;

    if (count > MaxRepeatCount)
    {
        throw Exceptions::MessageException(
            Message::MessageItem{
            Message::MessageKind::Error,
            Message::MessageID::TooLargeRepeatCount,
            m_IR.Name,
            location,
            {std::to_string(count), std::to_string(MaxRepeatCount)}
        }
        );
    }
}

} // namespace AST2IR

} // namespace YAMML
