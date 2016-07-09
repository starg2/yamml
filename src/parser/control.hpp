
#pragma once

#include <pegtl.hh>
#include <pegtl/contrib/changes.hh>

#include "action_state_attribute.hpp"
#include "action_state_composition.hpp"
#include "action_state_literal.hpp"
#include "action_state_phrase.hpp"
#include "error_control.hpp"
#include "parser_attribute.hpp"
#include "parser_composition.hpp"
#include "parser_literal.hpp"
#include "parser_phrase.hpp"

namespace YAMML
{

namespace Parser
{

template<typename TRule>
class Control : public ErrorControl<TRule>
{
};

template<>
class Control<Grammar::Composition>
    : public pegtl::change_state_and_action<
        Grammar::Composition,
        CompositionState,
        CompositionAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::Attribute>
    : public pegtl::change_state_and_action<
        Grammar::Attribute,
        AttributeState,
        AttributeAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::AttributeArgument>
    : public pegtl::change_state_and_action<
        Grammar::AttributeArgument,
        AttributeArgumentState,
        AttributeArgumentAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::AttributeKey>
    : public pegtl::change_action<
        Grammar::AttributeKey,
        AttributeKeyAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::Value>
    : public pegtl::change_state_and_action<
        Grammar::Value,
        ValueState,
        ValueAction,
        ErrorControl
    >
{
};

template<char TQuote>
class Control<Grammar::QuotedStringLiteral<TQuote>>
    : public pegtl::change_state_and_action<
        Grammar::QuotedStringLiteral<TQuote>,
        EscapeState,
        EscapeAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::TrackListBlock>
    : public pegtl::change_state_and_action<
        Grammar::TrackListBlock,
        TrackListBlockState,
        TrackListBlockAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::TrackBlock>
    : public pegtl::change_state_and_action<
        Grammar::TrackBlock,
        TrackBlockState,
        TrackBlockAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::TrackItem>
    : public pegtl::change_state_and_action<
        Grammar::TrackItem,
        TrackItemState,
        TrackItemAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::Command>
    : public pegtl::change_state_and_action<
        Grammar::Command,
        CommandState,
        CommandAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::CommandArgument>
    : public pegtl::change_state_and_action<
        Grammar::CommandArgument,
        CommandArgumentState,
        CommandArgumentAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::Phrase>
    : public pegtl::change_state_and_action<
        Grammar::Phrase,
        PhraseState,
        PhraseAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::NoteSequenceBlock>
    : public pegtl::change_state_and_action<
        Grammar::NoteSequenceBlock,
        NoteSequenceBlockState,
        NoteSequenceBlockAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::NoteSequenceBlockWithoutAttributes>
    : public pegtl::change_state_and_action<
        Grammar::NoteSequenceBlockWithoutAttributes,
        NoteSequenceBlockWithoutAttributesState,
        NoteSequenceBlockWithoutAttributesAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::NoteSequenceStatement>
    : public pegtl::change_state_and_action<
        Grammar::NoteSequenceStatement,
        NoteSequenceStatementState,
        NoteSequenceStatementAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::NoteSequence>
    : public pegtl::change_state_and_action<
        Grammar::NoteSequence,
        NoteSequenceState,
        NoteSequenceAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::NoteAndExpression>
    : public pegtl::change_state_and_action<
        Grammar::NoteAndExpression,
        NoteAndExpressionState,
        NoteAndExpressionAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::NoteRepeatEachExpression>
    : public pegtl::change_state_and_action<
        Grammar::NoteRepeatEachExpression,
        NoteRepeatEachExpressionState,
        NoteRepeatEachExpressionAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::NoteRepeatExpression>
    : public pegtl::change_state_and_action<
        Grammar::NoteRepeatExpression,
        NoteRepeatExpressionState,
        NoteRepeatExpressionAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::NoteAndDuration>
    : public pegtl::change_state_and_action<
        Grammar::NoteAndDuration,
        NoteAndDurationState,
        NoteAndDurationAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::DurationSet>
    : public pegtl::change_state_and_action<
        Grammar::DurationSet,
        DurationSetState,
        DurationSetAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::SimpleDurationWithModifier>
    : public pegtl::change_state_and_action<
        Grammar::SimpleDurationWithModifier,
        SimpleDurationWithModifierState,
        SimpleDurationWithModifierAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::SimpleDurationModifier>
    : public pegtl::change_state_and_action<
        Grammar::SimpleDurationModifier,
        SimpleDurationModifierState,
        SimpleDurationModifierAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::SimpleDuration>
    : public pegtl::change_state_and_action<
        Grammar::SimpleDuration,
        SimpleDurationState,
        SimpleDurationAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::SimpleChord>
    : public pegtl::change_state_and_action<
        Grammar::SimpleChord,
        SimpleChordState,
        SimpleChordAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::NoteNumber>
    : public pegtl::change_state_and_action<
        Grammar::NoteNumber,
        NoteNumberState,
        NoteNumberAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::NoteOctave>
    : public pegtl::change_state_and_action<
        Grammar::NoteOctave,
        NoteOctaveState,
        NoteOctaveAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::NoteName>
    : public pegtl::change_state_and_action<
        Grammar::NoteName,
        NoteNameState,
        NoteNameAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::Rest>
    : public pegtl::change_state_and_action<
        Grammar::Rest,
        RestState,
        RestAction,
        ErrorControl
    >
{
};

} // namespace Parser

} // namespace YAMML
