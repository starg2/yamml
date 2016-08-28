
#pragma once

#include <algorithm>
#include <string>
#include <type_traits>
#include <utility>

#include <pegtl.hh>

#include <ast/module.hpp>
#include <ast/phrase.hpp>
#include <parser/parser.hpp>

#include "action.hpp"
#include "parser_phrase.hpp"

namespace YAMML
{

namespace Parser
{

template<typename T>
AST::NoteSequence MakeNoteSequence(T node)
{
    AST::NoteAndExpression nae;
    nae.Location = node.Location;
    nae.Notes.emplace_back(node);

    AST::NoteSequence ns;
    ns.Location = node.Location;
    ns.Notes.push_back(std::move(nae));

    return ns;
}

class PhraseState
{
public:
    template<typename... TCommonStates>
    void success(AST::Module& mod, TCommonStates&...)
    {
        mod.Phrases.push_back(ASTNode);
    }

    AST::Phrase ASTNode;
};

template<typename TRule>
class PhraseAction : public pegtl::nothing<TRule>
{
};

template<>
class PhraseAction<Grammar::Phrase> : public AssignLocationAction
{
};

template<>
class PhraseAction<Grammar::Identifier>
{
public:
    template<typename... TCommonStates>
    static void apply(const pegtl::input& in, PhraseState& st, TCommonStates&...)
    {
        st.ASTNode.Name = in.string();
    }
};

class NoteSequenceBlockState
{
public:
    template<typename TParentState, typename... TCommonStates>
    void success(TParentState& st, TCommonStates&...)
    {
        st.ASTNode.Sequences.emplace_back(ASTNode);
    }

    AST::NoteSequenceBlock ASTNode;
};

template<typename TRule>
class NoteSequenceBlockAction : public pegtl::nothing<TRule>
{
};

template<>
class NoteSequenceBlockAction<Grammar::NoteSequenceBlock> : public AssignLocationAction
{
};

class NoteSequenceBlockWithoutAttributesState
{
public:
    template<typename TParentState, typename... TCommonStates>
    void success(TParentState& st, TCommonStates&...)
    {
        st.ASTNode.Block = ASTNode;
    }

    AST::NoteSequenceBlockWithoutAttributes ASTNode;
};

template<typename TRule>
class NoteSequenceBlockWithoutAttributesAction : public pegtl::nothing<TRule>
{
};

template<>
class NoteSequenceBlockWithoutAttributesAction<Grammar::NoteSequenceBlockWithoutAttributes> : public AssignLocationAction
{
};

class NoteSequenceStatementState
{
public:
    template<typename TParentState, typename... TCommonStates>
    void success(TParentState& st, TCommonStates&...)
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
class NoteSequenceStatementAction<Grammar::NoteSequence> : public AssignLocationAction
{
};

class NoteSequenceState
{
public:
    template<typename TParentState, typename... TCommonStates>
    void success(TParentState& st, TCommonStates&...)
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
class NoteSequenceAction<Grammar::NoteSequence> : public AssignLocationAction
{
};

class NoteAndExpressionState
{
public:
    template<typename TParentState, typename... TCommonStates>
    void success(TParentState& st, TCommonStates&...)
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
class NoteAndExpressionAction<Grammar::NoteAndExpression> : public AssignLocationAction
{
};

class NoteRepeatEachExpressionState
{
public:
    template<typename TParentState, typename... TCommonStates>
    void success(TParentState& st, TCommonStates&...)
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
class NoteRepeatEachExpressionAction<Grammar::NoteRepeatEachExpression> : public AssignLocationAction
{
};

template<>
class NoteRepeatEachExpressionAction<Grammar::UnsignedInteger>
{
public:
    template<typename... TCommonStates>
    static void apply(const pegtl::input& in, NoteRepeatEachExpressionState& st, TCommonStates&...)
    {
        st.ASTNode.Count = std::stoul(in.string());
    }
};

class NoteRepeatExpressionState
{
public:
    template<typename TParentState, typename... TCommonStates>
    void success(TParentState& st, TCommonStates&...)
    {
        st.ASTNode.Notes.emplace_back(MakeNoteSequence(ASTNode));
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
class NoteRepeatExpressionAction<Grammar::NoteRepeatExpression> : public AssignLocationAction
{
};

template<>
class NoteRepeatExpressionAction<Grammar::UnsignedInteger>
{
public:
    template<typename... TCommonStates>
    static void apply(const pegtl::input& in, NoteRepeatExpressionState& st, TCommonStates&...)
    {
        st.ASTNode.Count = std::stoul(in.string());
    }
};

class NoteAndDurationState
{
public:
    template<typename TParentState, typename... TCommonStates>
    void success(TParentState& st, TCommonStates&...)
    {
        st.ASTNode.Notes.emplace_back(MakeNoteSequence(ASTNode));
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
class NoteAndDurationAction<Grammar::NoteAndDuration> : public AssignLocationAction
{
};

class NoteAccentsState
{
public:
    template<typename TParentState, typename... TCommonStates>
    void success(TParentState& st, TCommonStates&...)
    {
        st.ASTNode.Accents = ASTNode;
    }

    AST::NoteAccents ASTNode;
};

template<typename TRule>
class NoteAccentsAction : public pegtl::nothing<TRule>
{
};

template<>
class NoteAccentsAction<Grammar::NoteAccents>
{
public:
    template<typename... TCommonStates>
    static void apply(const pegtl::input& in, NoteAccentsState& st, TCommonStates&...)
    {
        AssignLocationAction::apply(in, st);

        st.ASTNode.Accents = static_cast<int>(std::count(in.begin(), in.end(), '!'));
        st.ASTNode.Staccato = static_cast<int>(std::count(in.begin(), in.end(), '^'));
        st.ASTNode.Tenuto = static_cast<int>(std::count(in.begin(), in.end(), '~'));
    }
};

class DurationSetState
{
public:
    template<typename TParentState, typename... TCommonStates>
    void success(TParentState& st, TCommonStates&...)
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
class DurationSetAction<Grammar::DurationSet> : public AssignLocationAction
{
};

class SimpleDurationWithModifierState
{
public:
    template<typename TParentState, typename... TCommonStates>
    void success(TParentState& st, TCommonStates&...)
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
class SimpleDurationWithModifierAction<Grammar::SimpleDurationWithModifier> : public AssignLocationAction
{
};

class SimpleDurationModifierDotsState
{
public:
    template<typename TParentState, typename... TCommonStates>
    void success(TParentState& st, TCommonStates&...)
    {
        st.ASTNode.Modifier = ASTNode;
    }

    AST::SimpleDurationModifierDots ASTNode;
};

template<typename TRule>
class SimpleDurationModifierDotsAction : public pegtl::nothing<TRule>
{
};

template<>
class SimpleDurationModifierDotsAction<Grammar::SimpleDurationModifierDots>
{
public:
    template<typename... TCommonStates>
    static void apply(const pegtl::input& in, SimpleDurationModifierDotsState& st, TCommonStates&... commonStates)
    {
        AssignLocationAction::apply(in, st, commonStates...);
        st.ASTNode.Count = static_cast<unsigned long>(in.size());
    }
};

class SimpleDurationModifierState
{
public:
    template<typename TParentState, typename... TCommonStates>
    void success(TParentState& st, TCommonStates&...)
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
class SimpleDurationModifierAction<Grammar::SimpleDurationModifier> : public AssignLocationAction
{
};

template<>
class SimpleDurationModifierAction<Grammar::UnsignedInteger>
{
public:
    template<typename... TCommonStates>
    static void apply(const pegtl::input& in, SimpleDurationModifierState& st, TCommonStates&...)
    {
        st.ASTNode.Number = std::stoul(in.string());
    }
};

class SimpleDurationState
{
public:
    template<typename TParentState, typename... TCommonStates>
    void success(TParentState& st, TCommonStates&...)
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
class SimpleDurationAction<Grammar::SimpleDuration> : public AssignLocationAction
{
};

template<>
class SimpleDurationAction<Grammar::UnsignedInteger>
{
public:
    template<typename... TCommonStates>
    static void apply(const pegtl::input& in, SimpleDurationState& st, TCommonStates&...)
    {
        st.ASTNode.Number = std::stoul(in.string());
    }
};

class SimpleChordState
{
public:
    template<typename TParentState, typename... TCommonStates>
    void success(TParentState& st, TCommonStates&...)
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
class SimpleChordAction<Grammar::SimpleChord> : public AssignLocationAction
{
};

class NoteNumberState
{
public:
    template<typename TParentState, typename... TCommonStates>
    void success(TParentState& st, TCommonStates&...)
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
class NoteNumberAction<Grammar::NoteNumber> : public AssignLocationAction
{
};

class NoteOctaveState
{
public:
    template<typename TParentState, typename... TCommonStates>
    void success(TParentState& st, TCommonStates&...)
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
class NoteOctaveAction<Grammar::NoteOctave> : public AssignLocationAction
{
};

template<>
class NoteOctaveAction<Grammar::SignedInteger>
{
public:
    template<typename... TCommonStates>
    static void apply(const pegtl::input& in, NoteOctaveState& st, TCommonStates&...)
    {
        st.ASTNode.Value = std::stoi(in.string());
    }
};

class NoteNameState
{
public:
    NoteNameState() : ASTNode{}
    {
    }

    template<typename TParentState, typename... TCommonStates>
    void success(TParentState& st, TCommonStates&...)
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
class NoteNameAction<Grammar::NoteName> : public AssignLocationAction
{
};

template<>
class NoteNameAction<Grammar::NoteNameBase>
{
public:
    template<typename... TCommonStates>
    static void apply(const pegtl::input& in, NoteNameState& st, TCommonStates&...)
    {
        st.ASTNode.Name = in.string().at(0);
    }
};

template<>
class NoteNameAction<Grammar::NoteNameKey>
{
public:
    template<typename... TCommonStates>
    static void apply(const pegtl::input& in, NoteNameState& st, TCommonStates&...)
    {
        st.ASTNode.Minor = 0;

        st.ASTNode.Minor += static_cast<int>(std::count(in.begin(), in.end(), 's'));
        st.ASTNode.Minor -= static_cast<int>(std::count(in.begin(), in.end(), 'f'));
    }
};

class RestState
{
public:
    template<typename TParentState, typename... TCommonStates>
    void success(TParentState& st, TCommonStates&...)
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
class RestAction<Grammar::Rest> : public AssignLocationAction
{
};

} // namespace Parser

} // namespace YAMML
