
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

class SimpleDurationWithModifier : public pegtl::seq<SimpleDuration, pegtl::opt<pegtl::if_must<pegtl::one<'.'>, SimpleDurationModifier>>>
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

class SpaceDelimitedChords : public pegtl::pad<SimpleChord, Separator>
{
};

class NoteAndDuration
    : public pegtl::seq<
        pegtl::sor<
            pegtl::if_must<pegtl::one<'('>, SpaceDelimitedChords, pegtl::one<')'>>,
            Rest,
            NoteNumber
        >,
        pegtl::pad_opt<
            pegtl::if_must<pegtl::pad<pegtl::one<'/'>, Separator>, DurationOrDurationSet>,
            Separator
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

template<char TOperator>
class PaddedOperator : public pegtl::pad<pegtl::one<TOperator>, Separator>
{
};

template<char TOperator, typename... TChildren>
class NoteRepeatExpressionBase
    : public pegtl::if_must<
        UnsignedInteger,
        PaddedOperator<TOperator>,
        pegtl::sor<NoteSequenceInParentheses, TChildren...>
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
        pegtl::sor<NoteSequenceInParentheses, NoteRepeatEachExpression, NoteRepeatExpression, NoteAndDuration>,
        pegtl::one<'&'>,
        Separator
    >
{
};

class NoteSequence : public pegtl::star<pegtl::pad<NoteAndExpression, Separator>>
{
};

class NoteSequenceStatement : public pegtl::seq<AttributeOptionalSequence, pegtl::pad_opt<NoteSequence, Separator>, pegtl::one<';'>>
{
};

class NoteSequenceBlock;

class NoteSequenceStatementsAndBlocks : public pegtl::star<pegtl::sor<NoteSequenceStatement, NoteSequenceBlock>>
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
            pegtl_string_t("phrase"),
            Separator,
            PhraseName,
            NoteSequenceBlockWithoutAttributes
        >
    >
{
};

} // namespace Grammar

} // namespace Parser

} // namespace YAMML
