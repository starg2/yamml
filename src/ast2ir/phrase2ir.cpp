
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

constexpr int TickPerQuarter = 960;

class DurationCalculator final : public boost::static_visitor<>
{
public:
    void operator()(const AST::SimpleDurationWithModifier& ast)
    {
        if (ast.Modifier.is_initialized())
        {
            m_Duration += TickPerQuarter * 4 / (ast.Base.Number / 2) / ast.Modifier.get().Number;
        }
        else
        {
            m_Duration += TickPerQuarter * 4 / ast.Base.Number;
        }
    }

    void operator()(const AST::DurationSet& ast)
    {
        for (auto&& i : ast.Durations)
        {
            (*this)(i);
        }
    }

    int GetDuration() const
    {
        return m_Duration;
    }

private:
    int m_Duration = 0;
};

int CalculateDuration(const AST::NoteAndDuration& ast)
{
    if (ast.Duration.is_initialized())
    {
        DurationCalculator dc;
        ast.Duration->apply_visitor(dc);
        return dc.GetDuration();
    }
    else
    {
        return TickPerQuarter;
    }
}

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
    catch (const Exceptions::MessageException& e)
    {
        AddMessage(e.Item);
        return false;
    }
    catch (const std::exception& e)
    {
        AddMessage(
            Message::MessageItem{
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

std::vector<IR::Block::EventType> Phrase2IRCompiler::operator()(const AST::NoteSequenceStatement& ast)
{
    if (ast.Attributes.empty())
    {
        return (*this)(*ast.NoteSeq);
    }
    else
    {
        auto newIndex = AllocBlock();

        m_AttributeStack.push_back(ast.Attributes);
        AutoPop<decltype(m_AttributeStack)> autoPop(m_AttributeStack);

        // with bounds checking
        m_IR.Blocks[newIndex.ID].Attributes = m_AttributeStack.back();

        if (ast.NoteSeq.is_initialized())
        {
            auto events = (*this)(*ast.NoteSeq);
            m_IR.Blocks[newIndex.ID].Events.insert(m_IR.Blocks[newIndex.ID].Events.end(), events.begin(), events.end());
        }

        return {newIndex};
    }
}

std::vector<IR::Block::EventType> Phrase2IRCompiler::operator()(const AST::NoteSequenceBlock& ast)
{
    auto newIndex = AllocBlock();

    m_AttributeStack.push_back(ast.Attributes);
    AutoPop<decltype(m_AttributeStack)> autoPop(m_AttributeStack);

    Compile(ast.Block, newIndex);
    return {newIndex};
}

std::vector<IR::Block::EventType> Phrase2IRCompiler::operator()(const AST::NoteSequence& ast)
{
    std::vector<IR::Block::EventType> ret;

    for (auto&& i : ast.Notes)
    {
        auto events = (*this)(i);
        ret.insert(ret.end(), events.begin(), events.end());
    }

    return ret;
}

std::vector<IR::Block::EventType> Phrase2IRCompiler::operator()(const AST::NoteAndExpression& ast)
{
    std::vector<IR::Block::EventType> ret;

    int startTime = m_RelativeTime;
    int endTime = startTime;

    for (auto&& i : ast.Notes)
    {
        auto events = i.apply_visitor(*this);
        ret.insert(ret.end(), events.begin(), events.end());

        endTime = std::max(endTime, m_RelativeTime);
        m_RelativeTime = startTime;
    }

    m_RelativeTime = endTime;
    return ret;
}

std::vector<IR::Block::EventType> Phrase2IRCompiler::operator()(const AST::NoteAndDuration& ast)
{
    int duration = CalculateDuration(ast);
    boost::variant<int> varDuration = duration;

    auto newEvent = boost::apply_visitor(*this, ast.Note, varDuration);

    m_RelativeTime += duration;
    return newEvent;
}

std::vector<IR::Block::EventType> Phrase2IRCompiler::operator()(const AST::NoteRepeatExpression& ast)
{
    std::vector<IR::Block::EventType> ret;

    LimitRepeatCount(ast.Count, ast.Location);

    for (std::size_t i = 0; i < ast.Count; i++)
    {
        for (auto&& j : ast.Notes)
        {
            auto events = (*this)(j.get());
            ret.insert(ret.end(), events.begin(), events.end());
        }
    }

    return ret;
}

std::vector<IR::Block::EventType> Phrase2IRCompiler::operator()(const AST::NoteRepeatEachExpression& ast)
{
    std::vector<IR::Block::EventType> ret;

    LimitRepeatCount(ast.Count, ast.Location);

    for (auto&& i : ast.Notes)
    {
        for (auto&& j : i.get().Notes)
        {
            for (std::size_t k = 0; k < ast.Count; k++)
            {
                auto events = (*this)(j);
                ret.insert(ret.end(), events.begin(), events.end());
            }
        }
    }

    return ret;
}

std::vector<IR::Block::EventType> Phrase2IRCompiler::operator()(const AST::Rest& ast, int duration)
{
    static_cast<void>(ast);
    static_cast<void>(duration);

    return {IR::Event{m_RelativeTime, IR::Rest{}}};
}

std::vector<IR::Block::EventType> Phrase2IRCompiler::operator()(const AST::NoteNumber& ast, int duration)
{
    return {
        IR::Event{
            m_RelativeTime,
            IR::Note{
                MIDI::NoteNumber(ast.Name.Name, ast.Name.Minor, ast.Octave.get_value_or(AST::NoteOctave{4, ast.Location}).Value),
                100,
                duration,
                100
            }
        }
    };
}

std::vector<IR::Block::EventType> Phrase2IRCompiler::operator()(const AST::SimpleChord& ast, int duration)
{
    std::vector<IR::Block::EventType> ret;

    for (auto&& i : ast.Notes)
    {
        auto events = (*this)(i, duration);
        ret.insert(ret.end(), events.begin(), events.end());
    }

    return ret;
}

IR::BlockReference Phrase2IRCompiler::AllocBlock()
{
    auto newIndex = IR::BlockReference{m_IR.Blocks.size()};
    m_IR.Blocks.emplace_back();
    return newIndex;
}

void Phrase2IRCompiler::Compile(const AST::NoteSequenceBlockWithoutAttributes& ast, IR::BlockReference index)
{
    // with bounds checking
    m_IR.Blocks.at(index.ID).Attributes = m_AttributeStack.back();

    for (auto&& i : ast.Sequences)
    {
        auto events = i.apply_visitor(*this);
        m_IR.Blocks[index.ID].Events.insert(m_IR.Blocks[index.ID].Events.end(), events.begin(), events.end());
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
