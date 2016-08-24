
#pragma once

#include <pegtl.hh>

#include "action_state_attribute.hpp"
#include "action_state_composition.hpp"
#include "action_state_literal.hpp"
#include "action_state_phrase.hpp"

#include "changes.hpp"

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
    : public ChangeStateAndAction<
        Grammar::Composition,
        CompositionState,
        CompositionAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::Attribute>
    : public ChangeStateAndAction<
        Grammar::Attribute,
        AttributeState,
        AttributeAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::AttributeArgument>
    : public ChangeStateAndAction<
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
    : public ChangeStateAndAction<
        Grammar::Value,
        ValueState,
        ValueAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::DoubleQuotedStringLiteral>
    : public ChangeStateAndAction<
    Grammar::DoubleQuotedStringLiteral,
    EscapeState,
    EscapeAction,
    ErrorControl
    >
{
};

template<>
class Control<Grammar::SingleQuotedStringLiteral>
    : public ChangeStateAndAction<
    Grammar::SingleQuotedStringLiteral,
    EscapeState,
    EscapeAction,
    ErrorControl
    >
{
};

template<>
class Control<Grammar::TrackListBlock>
    : public ChangeStateAndAction<
        Grammar::TrackListBlock,
        TrackListBlockState,
        TrackListBlockAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::TrackBlock>
    : public ChangeStateAndAction<
        Grammar::TrackBlock,
        TrackBlockState,
        TrackBlockAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::TrackItem>
    : public ChangeStateAndAction<
        Grammar::TrackItem,
        TrackItemState,
        TrackItemAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::Command>
    : public ChangeStateAndAction<
        Grammar::Command,
        CommandState,
        CommandAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::CommandArgument>
    : public ChangeStateAndAction<
        Grammar::CommandArgument,
        CommandArgumentState,
        CommandArgumentAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::Phrase>
    : public ChangeStateAndAction<
        Grammar::Phrase,
        PhraseState,
        PhraseAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::NoteSequenceBlock>
    : public ChangeStateAndAction<
        Grammar::NoteSequenceBlock,
        NoteSequenceBlockState,
        NoteSequenceBlockAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::NoteSequenceBlockWithoutAttributes>
    : public ChangeStateAndAction<
        Grammar::NoteSequenceBlockWithoutAttributes,
        NoteSequenceBlockWithoutAttributesState,
        NoteSequenceBlockWithoutAttributesAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::NoteSequenceStatement>
    : public ChangeStateAndAction<
        Grammar::NoteSequenceStatement,
        NoteSequenceStatementState,
        NoteSequenceStatementAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::NoteSequence>
    : public ChangeStateAndAction<
        Grammar::NoteSequence,
        NoteSequenceState,
        NoteSequenceAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::NoteAndExpression>
    : public ChangeStateAndAction<
        Grammar::NoteAndExpression,
        NoteAndExpressionState,
        NoteAndExpressionAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::NoteRepeatEachExpression>
    : public ChangeStateAndAction<
        Grammar::NoteRepeatEachExpression,
        NoteRepeatEachExpressionState,
        NoteRepeatEachExpressionAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::NoteRepeatExpression>
    : public ChangeStateAndAction<
        Grammar::NoteRepeatExpression,
        NoteRepeatExpressionState,
        NoteRepeatExpressionAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::NoteAndDuration>
    : public ChangeStateAndAction<
        Grammar::NoteAndDuration,
        NoteAndDurationState,
        NoteAndDurationAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::DurationSet>
    : public ChangeStateAndAction<
        Grammar::DurationSet,
        DurationSetState,
        DurationSetAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::SimpleDurationWithModifier>
    : public ChangeStateAndAction<
        Grammar::SimpleDurationWithModifier,
        SimpleDurationWithModifierState,
        SimpleDurationWithModifierAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::SimpleDurationModifierDots>
    : public ChangeStateAndAction<
        Grammar::SimpleDurationModifierDots,
        SimpleDurationModifierDotsState,
        SimpleDurationModifierDotsAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::SimpleDurationModifier>
    : public ChangeStateAndAction<
        Grammar::SimpleDurationModifier,
        SimpleDurationModifierState,
        SimpleDurationModifierAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::SimpleDuration>
    : public ChangeStateAndAction<
        Grammar::SimpleDuration,
        SimpleDurationState,
        SimpleDurationAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::SimpleChord>
    : public ChangeStateAndAction<
        Grammar::SimpleChord,
        SimpleChordState,
        SimpleChordAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::NoteNumber>
    : public ChangeStateAndAction<
        Grammar::NoteNumber,
        NoteNumberState,
        NoteNumberAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::NoteOctave>
    : public ChangeStateAndAction<
        Grammar::NoteOctave,
        NoteOctaveState,
        NoteOctaveAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::NoteName>
    : public ChangeStateAndAction<
        Grammar::NoteName,
        NoteNameState,
        NoteNameAction,
        ErrorControl
    >
{
};

template<>
class Control<Grammar::Rest>
    : public ChangeStateAndAction<
        Grammar::Rest,
        RestState,
        RestAction,
        ErrorControl
    >
{
};

} // namespace Parser

} // namespace YAMML
