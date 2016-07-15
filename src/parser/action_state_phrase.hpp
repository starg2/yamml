
#pragma once

#include <algorithm>
#include <string>
#include <type_traits>

#include <pegtl.hh>

#include <ast/module.hpp>
#include <ast/phrase.hpp>

#include "parser_phrase.hpp"

namespace YAMML
{

namespace Parser
{

class PhraseState
{
public:
    void success(AST::Module& mod)
    {
        mod.Add(ASTNode);
    }

    void OnParse(AST::NoteSequenceBlockWithoutAttributes node)
    {
        ASTNode.Block = node;
    }

    AST::Phrase ASTNode;
};

template<typename TRule>
class PhraseAction : public pegtl::nothing<TRule>
{
};

template<>
class PhraseAction<Grammar::Phrase>
{
public:
    static void apply(const pegtl::input& in, PhraseState& st)
    {
        st.ASTNode.Location = {in.line(), in.column()};
    }
};

template<>
class PhraseAction<Grammar::Identifier>
{
public:
    static void apply(const pegtl::input& in, PhraseState& st)
    {
        st.ASTNode.Name = in.string();
    }
};

class NoteSequenceBlockState
{
public:
    template<typename TState>
    void success(TState& st)
    {
        st.ASTNode.Sequences.emplace_back(ASTNode);
    }

    void OnParse(AST::NoteSequenceBlockWithoutAttributes node)
    {
        ASTNode.Sequences.emplace_back(node);
    }

    AST::NoteSequenceBlock ASTNode;
};

template<typename TRule>
class NoteSequenceBlockAction : public pegtl::nothing<TRule>
{
};

template<>
class NoteSequenceBlockAction<Grammar::NoteSequenceBlock>
{
public:
    static void apply(const pegtl::input& in, NoteSequenceBlockState& st)
    {
        st.ASTNode.Location = {in.line(), in.column()};
    }
};

class NoteSequenceBlockWithoutAttributesState
{
public:
    template<typename TState>
    void success(TState& st)
    {
        st.OnParse(ASTNode);
    }

    AST::NoteSequenceBlockWithoutAttributes ASTNode;
};

template<typename TRule>
class NoteSequenceBlockWithoutAttributesAction : public pegtl::nothing<TRule>
{
};

template<>
class NoteSequenceBlockWithoutAttributesAction<Grammar::NoteSequenceBlockWithoutAttributes>
{
public:
    static void apply(const pegtl::input& in, NoteSequenceBlockWithoutAttributesState& st)
    {
        st.ASTNode.Location = {in.line(), in.column()};
    }
};

class NoteSequenceStatementState
{
public:
    template<typename TState>
    void success(TState& st)
    {
        st.ASTNode.Sequences.emplace_back(ASTNode);
    }

    void OnParse(AST::NoteSequence node)
    {
        ASTNode.NoteSeq = node;
    }

    AST::NoteSequenceStatement ASTNode;
};

template<typename TRule>
class NoteSequenceStatementAction : public pegtl::nothing<TRule>
{
};

template<>
class NoteSequenceStatementAction<Grammar::NoteSequence>
{
public:
    static void apply(const pegtl::input& in, NoteSequenceStatementState& st)
    {
        st.ASTNode.Location = {in.line(), in.column()};
    }
};

class NoteSequenceState
{
public:
    template<typename TState>
    void success(TState& st)
    {
        st.OnParse(ASTNode);
    }

    AST::NoteSequence ASTNode;
};

template<typename TRule>
class NoteSequenceAction : public pegtl::nothing<TRule>
{
};

template<>
class NoteSequenceAction<Grammar::NoteSequence>
{
public:
    static void apply(const pegtl::input& in, NoteSequenceState& st)
    {
        st.ASTNode.Location = {in.line(), in.column()};
    }
};

class NoteAndExpressionState
{
public:
    template<typename TState>
    void success(TState& st)
    {
        st.ASTNode.Notes.push_back(ASTNode);
    }

    void OnParse(AST::NoteSequence node)
    {
        ASTNode.Notes.emplace_back(node);
    }

    AST::NoteAndExpression ASTNode;
};

template<typename TRule>
class NoteAndExpressionAction : public pegtl::nothing<TRule>
{
};

template<>
class NoteAndExpressionAction<Grammar::NoteAndExpression>
{
public:
    static void apply(const pegtl::input& in, NoteAndExpressionState& st)
    {
        st.ASTNode.Location = {in.line(), in.column()};
    }
};

class NoteRepeatEachExpressionState
{
public:
    template<typename TState>
    void success(TState& st)
    {
        st.ASTNode.Notes.emplace_back(ASTNode);
    }

    void OnParse(AST::NoteSequence node)
    {
        ASTNode.Notes.emplace_back(node);
    }

    AST::NoteRepeatEachExpression ASTNode;
};

template<typename TRule>
class NoteRepeatEachExpressionAction : public pegtl::nothing<TRule>
{
};

template<>
class NoteRepeatEachExpressionAction<Grammar::NoteRepeatEachExpression>
{
public:
    static void apply(const pegtl::input& in, NoteRepeatEachExpressionState& st)
    {
        st.ASTNode.Location = {in.line(), in.column()};
    }
};

template<>
class NoteRepeatEachExpressionAction<Grammar::UnsignedInteger>
{
public:
    static void apply(const pegtl::input& in, NoteRepeatEachExpressionState& st)
    {
        st.ASTNode.Count = std::stoul(in.string());
    }
};

class NoteRepeatExpressionState
{
public:
    template<typename TState>
    void success(TState& st)
    {
        st.ASTNode.Notes.emplace_back(ASTNode);
    }

    void OnParse(AST::NoteSequence node)
    {
        ASTNode.Notes.emplace_back(node);
    }

    AST::NoteRepeatExpression ASTNode;
};

template<typename TRule>
class NoteRepeatExpressionAction : public pegtl::nothing<TRule>
{
};

template<>
class NoteRepeatExpressionAction<Grammar::NoteRepeatExpression>
{
public:
    static void apply(const pegtl::input& in, NoteRepeatExpressionState& st)
    {
        st.ASTNode.Location = {in.line(), in.column()};
    }
};

template<>
class NoteRepeatExpressionAction<Grammar::UnsignedInteger>
{
public:
    static void apply(const pegtl::input& in, NoteRepeatExpressionState& st)
    {
        st.ASTNode.Count = std::stoul(in.string());
    }
};

class NoteAndDurationState
{
public:
    template<typename TState>
    void success(TState& st)
    {
        st.ASTNode.Notes.emplace_back(ASTNode);
    }

    void OnParse(AST::SimpleDurationWithModifier node)
    {
        ASTNode.Duration = node;
    }

    void OnParse(AST::NoteNumber node)
    {
        ASTNode.Note = node;
    }

    AST::NoteAndDuration ASTNode;
};

template<typename TRule>
class NoteAndDurationAction : public pegtl::nothing<TRule>
{
};

template<>
class NoteAndDurationAction<Grammar::NoteAndDuration>
{
public:
    static void apply(const pegtl::input& in, NoteAndDurationState& st)
    {
        st.ASTNode.Location = {in.line(), in.column()};
    }
};

class DurationSetState
{
public:
    template<typename TState>
    void success(TState& st)
    {
        st.ASTNode.Duration = ASTNode;
    }

    void OnParse(AST::SimpleDurationWithModifier node)
    {
        ASTNode.Durations.push_back(node);
    }

    AST::DurationSet ASTNode;
};

template<typename TRule>
class DurationSetAction : public pegtl::nothing<TRule>
{
};

template<>
class DurationSetAction<Grammar::DurationSet>
{
public:
    static void apply(const pegtl::input& in, DurationSetState& st)
    {
        st.ASTNode.Location = {in.line(), in.column()};
    }
};

class SimpleDurationWithModifierState
{
public:
    template<typename TState>
    void success(TState& st)
    {
        st.OnParse(ASTNode);
    }

    AST::SimpleDurationWithModifier ASTNode;
};

template<typename TRule>
class SimpleDurationWithModifierAction : public pegtl::nothing<TRule>
{
};

template<>
class SimpleDurationWithModifierAction<Grammar::SimpleDurationWithModifier>
{
public:
    static void apply(const pegtl::input& in, SimpleDurationWithModifierState& st)
    {
        st.ASTNode.Location = {in.line(), in.column()};
    }
};

class SimpleDurationModifierState
{
public:
    template<typename TState>
    void success(TState& st)
    {
        st.ASTNode.Modifier = ASTNode;
    }

    AST::SimpleDurationModifier ASTNode;
};

template<typename TRule>
class SimpleDurationModifierAction : public pegtl::nothing<TRule>
{
};

template<>
class SimpleDurationModifierAction<Grammar::SimpleDurationModifier>
{
public:
    static void apply(const pegtl::input& in, SimpleDurationModifierState& st)
    {
        st.ASTNode.Location = {in.line(), in.column()};
    }
};

template<>
class SimpleDurationModifierAction<Grammar::UnsignedInteger>
{
public:
    static void apply(const pegtl::input& in, SimpleDurationModifierState& st)
    {
        st.ASTNode.Number = std::stoul(in.string());
    }
};

class SimpleDurationState
{
public:
    template<typename TState>
    void success(TState& st)
    {
        st.ASTNode.Base = ASTNode;
    }

    AST::SimpleDuration ASTNode;
};

template<typename TRule>
class SimpleDurationAction : public pegtl::nothing<TRule>
{
};

template<>
class SimpleDurationAction<Grammar::SimpleDuration>
{
public:
    static void apply(const pegtl::input& in, SimpleDurationState& st)
    {
        st.ASTNode.Location = {in.line(), in.column()};
    }
};

template<>
class SimpleDurationAction<Grammar::UnsignedInteger>
{
public:
    static void apply(const pegtl::input& in, SimpleDurationState& st)
    {
        st.ASTNode.Number = std::stoul(in.string());
    }
};

class SimpleChordState
{
public:
    template<typename TState>
    void success(TState& st)
    {
        st.ASTNode.Note = ASTNode;
    }

    void OnParse(AST::NoteNumber node)
    {
        ASTNode.Notes.push_back(node);
    }

    AST::SimpleChord ASTNode;
};

template<typename TRule>
class SimpleChordAction : public pegtl::nothing<TRule>
{
};

template<>
class SimpleChordAction<Grammar::SimpleChord>
{
public:
    static void apply(const pegtl::input& in, SimpleChordState& st)
    {
        st.ASTNode.Location = {in.line(), in.column()};
    }
};

class NoteNumberState
{
public:
    template<typename TState>
    void success(TState& st)
    {
        st.OnParse(ASTNode);
    }

    AST::NoteNumber ASTNode;
};

template<typename TRule>
class NoteNumberAction : public pegtl::nothing<TRule>
{
};

template<>
class NoteNumberAction<Grammar::NoteNumber>
{
public:
    static void apply(const pegtl::input& in, NoteNumberState& st)
    {
        st.ASTNode.Location = {in.line(), in.column()};
    }
};

class NoteOctaveState
{
public:
    template<typename TState>
    void success(TState& st)
    {
        st.ASTNode.Octave = ASTNode;
    }

    AST::NoteOctave ASTNode;
};

template<typename TRule>
class NoteOctaveAction : public pegtl::nothing<TRule>
{
};

template<>
class NoteOctaveAction<Grammar::NoteOctave>
{
public:
    static void apply(const pegtl::input& in, NoteOctaveState& st)
    {
        st.ASTNode.Location = {in.line(), in.column()};
    }
};

template<>
class NoteOctaveAction<Grammar::SignedInteger>
{
public:
    static void apply(const pegtl::input& in, NoteOctaveState& st)
    {
        st.ASTNode.Value = std::stoi(in.string());
    }
};

class NoteNameState
{
public:
    template<typename TState>
    void success(TState& st)
    {
        st.ASTNode.Name = ASTNode;
    }

    AST::NoteName ASTNode;
};

template<typename TRule>
class NoteNameAction : public pegtl::nothing<TRule>
{
};

template<>
class NoteNameAction<Grammar::NoteName>
{
public:
    static void apply(const pegtl::input& in, NoteNameState& st)
    {
        st.ASTNode.Location = {in.line(), in.column()};
    }
};

template<>
class NoteNameAction<Grammar::NoteNameBase>
{
public:
    static void apply(const pegtl::input& in, NoteNameState& st)
    {
        st.ASTNode.Name = in.string().at(0);
    }
};

template<>
class NoteNameAction<Grammar::NoteNameKey>
{
public:
    static void apply(const pegtl::input& in, NoteNameState& st)
    {
        st.ASTNode.Minor = 0;

        st.ASTNode.Minor += std::count(in.begin(), in.end(), 's');
        st.ASTNode.Minor -= std::count(in.begin(), in.end(), 'f');
    }
};

class RestState
{
public:
    template<typename TState>
    void success(TState& st)
    {
        st.ASTNode.Note = ASTNode;
    }

    AST::Rest ASTNode;
};

template<typename TRule>
class RestAction : public pegtl::nothing<TRule>
{
};

template<>
class RestAction<Grammar::Rest>
{
public:
    static void apply(const pegtl::input& in, RestState& st)
    {
        st.ASTNode.Location = {in.line(), in.column()};
    }
};

} // namespace Parser

} // namespace YAMML
