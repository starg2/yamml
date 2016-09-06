
#include <cassert>
#include <cmath>

#include <algorithm>
#include <deque>
#include <exception>
#include <string>
#include <typeinfo>
#include <vector>

#include <boost/optional.hpp>
#include <boost/variant.hpp>

#include <ast/attribute.hpp>
#include <ast/phrase.hpp>
#include <common/containerutil.hpp>
#include <compiler/nested.hpp>
#include <exceptions/messageexception.hpp>
#include <message/message.hpp>
#include <ir/block.hpp>
#include <ir/module.hpp>
#include <midi/limits.hpp>

#include "phrase2ir.hpp"

namespace YAMML
{

namespace AST2IR
{

class DurationCalculator final : public boost::static_visitor<int>
{
public:
    int operator()(const AST::SimpleDurationWithModifier& ast)
    {
        if (ast.Modifier.is_initialized())
        {
            if (ast.Modifier.value().type() == typeid(AST::SimpleDurationModifierDots))
            {
                long pow2 = std::lround(std::pow(2, boost::get<AST::SimpleDurationModifierDots>(ast.Modifier.value()).Count));
                return MIDI::TickPerQuarter * 4 / (ast.Base.Number / 2) - MIDI::TickPerQuarter * 4 / (ast.Base.Number * pow2);
            }
            else
            {
                return MIDI::TickPerQuarter * 4 / (ast.Base.Number / 2) / boost::get<AST::SimpleDurationModifier>(ast.Modifier.value()).Number;
            }
        }
        else
        {
            return MIDI::TickPerQuarter * 4 / ast.Base.Number;
        }
    }

    int operator()(const AST::DurationSet& ast)
    {
        int ret = 0;

        for (auto&& i : ast.Durations)
        {
            ret += (*this)(i);
        }

        return ret;
    }
};

Phrase2IRCompiler::Phrase2IRCompiler(Compiler::CompilerBase& parentCompiler, IR::Module& ir)
    : NestedCompilerBase(parentCompiler), m_IR(ir)
{
}

bool Phrase2IRCompiler::Compile(const AST::Phrase& ast, IR::BlockReference index)
{
    try
    {
        m_AttributeStack.push_back(ast.Attributes);
        Common::AutoPop<decltype(m_AttributeStack)> autoPop(m_AttributeStack);

        Compile(ast.Block, index);
        return !HasErrors();
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
    m_DefaultDuration = MIDI::TickPerQuarter;
    m_DefaultOctave = 5;

    if (ast.Attributes.empty())
    {
        return (*this)(ast.NoteSeq.value());
    }
    else
    {
        ResetTime();
        auto newIndex = AllocBlock();

        m_AttributeStack.push_back(ast.Attributes);
        Common::AutoPop<decltype(m_AttributeStack)> autoPop(m_AttributeStack);

        m_IR.Blocks[newIndex.ID].Attributes = m_AttributeStack.back();

        if (ast.NoteSeq.is_initialized())
        {
            auto events = (*this)(ast.NoteSeq.value());
            m_IR.Blocks[newIndex.ID].Events.insert(m_IR.Blocks[newIndex.ID].Events.end(), events.begin(), events.end());
        }

        ResetTime();
        return {newIndex};
    }
}

std::vector<IR::Block::EventType> Phrase2IRCompiler::operator()(const AST::NoteSequenceBlock& ast)
{
    ResetTime();
    auto newIndex = AllocBlock();

    m_AttributeStack.push_back(ast.Attributes);
    Common::AutoPop<decltype(m_AttributeStack)> autoPop(m_AttributeStack);

    Compile(ast.Block, newIndex);
    ResetTime();
    return {newIndex};
}

std::vector<IR::Block::EventType> Phrase2IRCompiler::operator()(const AST::NoteSequence& ast)
{
    std::vector<IR::Block::EventType> ret;

    for (auto&& i : ast.Notes)
    {
        auto events = i.apply_visitor(*this);
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
        auto events = (*this)(i);
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
    m_DefaultDuration = duration;

    boost::variant<DurationAndVelocity> varDV = DurationAndVelocity{
        GetNetDuration(duration, ast.Accents),
        GetVelocity(ast.Accents)
    };

    auto newEvent = boost::apply_visitor(*this, ast.Note, varDV);

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
            auto events = (*this)(j);
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
        for (auto&& j : i.Notes)
        {
            for (std::size_t k = 0; k < ast.Count; k++)
            {
                auto events = j.apply_visitor(*this);
                ret.insert(ret.end(), events.begin(), events.end());
            }
        }
    }

    return ret;
}

std::vector<IR::Block::EventType> Phrase2IRCompiler::operator()(const AST::Rest& ast, const DurationAndVelocity& dv)
{
    static_cast<void>(ast);
    return {IR::Event{m_RelativeTime, IR::Rest{dv.Duration}}};
}

std::vector<IR::Block::EventType> Phrase2IRCompiler::operator()(const AST::NoteNumber& ast, const DurationAndVelocity& dv)
{
    if (ast.Octave.is_initialized())
    {
        auto octave = ast.Octave.value();

        if (0 <= octave.Value && octave.Value <= 10)
        {
            m_DefaultOctave = octave.Value;
        }
        else
        {
            AddMessage(
                Message::MessageItem{
                    Message::MessageKind::Error,
                    Message::MessageID::OctaveOutOfRange,
                    m_IR.Name,
                    octave.Location,
                    {std::to_string(octave.Value)}
                }
            );
        }
    }

    return {
        IR::Event{
            m_RelativeTime,
            IR::Note{
                MIDI::NoteNumber(ast.Name.Name, ast.Name.Minor, m_DefaultOctave),
                dv.Velocity,
                dv.Duration,
                100
            }
        }
    };
}

std::vector<IR::Block::EventType> Phrase2IRCompiler::operator()(const AST::SimpleChord& ast, const DurationAndVelocity& dv)
{
    std::vector<IR::Block::EventType> ret;

    for (auto&& i : ast.Notes)
    {
        auto events = (*this)(i, dv);
        ret.insert(ret.end(), events.begin(), events.end());
    }

    return ret;
}

int Phrase2IRCompiler::CalculateDuration(const AST::NoteAndDuration& ast)
{
    if (ast.Duration.is_initialized())
    {
        DurationCalculator dc;
        return ast.Duration->apply_visitor(dc);
    }
    else
    {
        return m_DefaultDuration;
    }
}

int Phrase2IRCompiler::GetNetDuration(int duration, const boost::optional<AST::NoteAccents>& accents)
{
    if (accents.is_initialized())
    {
        int st = accents->Staccato - accents->Tenuto;

        switch (st)
        {
        case -1:
            return duration * 95 / 100;
        case 0:
            return duration * 9 / 10;
        case 1:
            return duration * 3 / 4;
        default:
            return (st < 0) ? duration : static_cast<int>(std::lround(duration * std::pow(2, -st + 1)));
        }
    }
    else
    {
        return duration * 9 / 10;
    }
}

int Phrase2IRCompiler::GetVelocity(const boost::optional<AST::NoteAccents>& accents)
{
    if (accents.is_initialized())
    {
        return std::min(static_cast<int>(std::lround(std::pow(1.2, accents.value().Accents) * 72)), 127);
    }
    else
    {
        return 72;
    }
}

void Phrase2IRCompiler::ResetTime()
{
    m_RelativeTime = 0;
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

    m_IR.Blocks[index.ID].Events.emplace_back(IR::Event{m_RelativeTime, IR::Rest{0}});
}

void Phrase2IRCompiler::LimitRepeatCount(std::size_t count, const AST::SourceLocation& location)
{
    constexpr std::size_t MaxRepeatCount = 100;

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
