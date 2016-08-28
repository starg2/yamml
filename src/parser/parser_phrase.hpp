
#pragma once

#include <pegtl.hh>

#include "parser_attribute.hpp"
#include "parser_common.hpp"
#include "parser_literal.hpp"
#include "parser_skips.hpp"

namespace YAMML
{

namespace Parser
{

namespace Grammar
{

class Rest : public pegtl::one<'R'>
{
};

class NoteNameBase : public pegtl::range<'A', 'G'>
{
};

class NoteNameKey : public pegtl::plus<pegtl::one<'s', 'f'>>
{
};

class NoteName : public pegtl::seq<NoteNameBase, pegtl::opt<NoteNameKey>>
{
};

class NoteOctave : public pegtl::seq<SignedInteger>
{
};

class NoteNumber : public pegtl::seq<NoteName, pegtl::opt<NoteOctave>>
{
};

class SimpleChord : public pegtl::list_must<NoteNumber, pegtl::one<'&'>, Separator>
{
};

class SimpleDuration : public pegtl::seq<UnsignedInteger>
{
};

class SimpleDurationModifier : public pegtl::seq<UnsignedInteger>
{
};

class SimpleDurationModifierDots : public pegtl::plus<pegtl::one<'.'>>
{
};

class SimpleDurationWithModifier
    : public pegtl::seq<
        SimpleDuration,
        pegtl::opt<
            pegtl::sor<
                pegtl::seq<pegtl::one<'.'>, SimpleDurationModifier>,
                SimpleDurationModifierDots
            >
        >
    >
{
};

class SpaceDelimitedDurations : public pegtl::plus<pegtl::pad<SimpleDurationWithModifier, Separator>>
{
};

class DurationSet : public pegtl::if_must<pegtl::one<'('>, SpaceDelimitedDurations, pegtl::one<')'>>
{
};

class DurationOrDurationSet : public pegtl::sor<DurationSet, SimpleDurationWithModifier>
{
};

class SlashAndDuration : public pegtl::if_must<pegtl::pad<pegtl::one<'/'>, Separator>, DurationOrDurationSet>
{
};

class SpaceDelimitedChords : public pegtl::pad<SimpleChord, Separator>
{
};

class SpaceDelimitedChordsInParenthesis : public pegtl::if_must<pegtl::one<'('>, SpaceDelimitedChords, pegtl::one<')'>>
{
};

class NoteAccents : public pegtl::plus<pegtl::one<'!', '^', '~'>>
{
};

class NoteAndDuration
    : public pegtl::sor<
        pegtl::seq<
            pegtl::sor<SpaceDelimitedChordsInParenthesis, NoteNumber>,
            pegtl::pad_opt<SlashAndDuration, Separator>,
            pegtl::pad_opt<NoteAccents, Separator>
        >,
        pegtl::seq<
            Rest,
            pegtl::pad_opt<SlashAndDuration, Separator>
        >
    >
{
};

class NoteSequence;

class NoteSequenceAndSeparators : public pegtl::pad<NoteSequence, Separator>
{
};

class NoteSequenceInParentheses : public pegtl::if_must<pegtl::one<'('>, NoteSequenceAndSeparators, pegtl::one<')'>>
{
};

class TrapMissingRepeatOperator : public pegtl::seq<UnsignedInteger, pegtl::raise<TrapMissingRepeatOperator>>
{
};

template<char TOperator>
class PaddedOperator : public pegtl::pad<pegtl::one<TOperator>, Separator>
{
};

template<char TOperator, typename... TChildren>
class NoteRepeatExpressionBase
    : public pegtl::seq<
        UnsignedInteger,
        pegtl::if_must<
            PaddedOperator<TOperator>,
            pegtl::sor<NoteSequenceInParentheses, TChildren...>
        >
    >
{
};

class NoteRepeatExpression : public NoteRepeatExpressionBase<'*', NoteAndDuration>
{
};

class NoteRepeatEachExpression : public NoteRepeatExpressionBase<'%', NoteRepeatExpression, NoteAndDuration>
{
};

class NoteAndExpression
    : public pegtl::list_must<
        pegtl::sor<NoteSequenceInParentheses, NoteRepeatEachExpression, NoteRepeatExpression, TrapMissingRepeatOperator, NoteAndDuration>,
        pegtl::one<'&'>,
        Separator
    >
{
};

class NoteSequence : public pegtl::plus<pegtl::pad<NoteAndExpression, Separator>>
{
};

class NoteSequenceStatement
    : public pegtl::seq<
    AttributeOptionalSequence,
    pegtl::if_must<
        NoteSequence,
        pegtl::one<';'>
    >
>
{
};

class NoteSequenceBlock;

class NoteSequenceStatementsAndBlocks : public pegtl::star<pegtl::sor<NoteSequenceBlock, NoteSequenceStatement>>
{
};

class NoteSequenceBlockWithoutAttributes
    : public pegtl::if_must<
        BlockBegin,
        NoteSequenceStatementsAndBlocks,
        BlockEnd
    >
{
};

class NoteSequenceBlock : public pegtl::seq<AttributeOptionalSequence, NoteSequenceBlockWithoutAttributes>
{
};

class PhraseName : public pegtl::pad<Identifier, Separator>
{
};

class Phrase
    : public pegtl::seq<
        AttributeOptionalSequence,
        pegtl::if_must<
            pegtl::seq<pegtl_string_t("phrase"), pegtl::sor<Separator, pegtl::eof>>,
            Separators,
            PhraseName,
            NoteSequenceBlockWithoutAttributes
        >
    >
{
};

} // namespace Grammar

} // namespace Parser

} // namespace YAMML
