
#pragma once

#include <pegtl.hh>

#include "parser_attribute.hpp"
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

class NoteOctave : public SignedInteger
{
};

class NoteNumber : public pegtl::seq<NoteName, pegtl::opt<NoteOctave>>
{
};

class SimpleChord : public pegtl::list_must<NoteNumber, pegtl::one<'&'>, Separator>
{
};

class SimpleDuration : public UnsignedInteger
{
};

class SimpleDurationModifier : public UnsignedInteger
{
};

class SimpleDurationWithModifier : public pegtl::seq<SimpleDuration, pegtl::opt<pegtl::if_must<pegtl::one<'.'>, SimpleDurationModifier>>>
{
};

class DurationSet : public pegtl::if_must<pegtl::one<'('>, pegtl::plus<pegtl::pad<SimpleDurationWithModifier, Separator>>, pegtl::one<')'>>
{
};

class NoteAndDuration
    : public pegtl::seq<
        pegtl::sor<
            pegtl::if_must<pegtl::one<'('>, pegtl::pad<SimpleChord, Separator>, pegtl::one<')'>>,
            Rest,
            NoteNumber
        >,
        pegtl::pad_opt<
            pegtl::if_must<pegtl::pad<pegtl::one<'/'>, Separator>, pegtl::sor<DurationSet, SimpleDurationWithModifier>>,
            Separator
        >
    >
{
};

class NoteSequence;

class NoteSequenceInParentheses : public pegtl::if_must<pegtl::one<'('>, pegtl::pad<NoteSequence, Separator>, pegtl::one<')'>>
{
};

template<char TOperator, typename... TChildren>
class NoteRepeatExpressionBase
    : public pegtl::seq<
        UnsignedInteger,
        pegtl::if_must<
            pegtl::pad<pegtl::one<TOperator>, Separator>,
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
        pegtl::sor<NoteAndDuration, NoteRepeatEachExpression, NoteRepeatExpression, NoteSequenceInParentheses>,
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

class NoteSequenceBlockWithoutAttributes
    : public pegtl::if_must<
        pegtl::one<'{'>,
        pegtl::star<pegtl::sor<NoteSequenceStatement, NoteSequenceBlock>>,
        Separators,
        pegtl::one<'}'>
    >
{
};

class NoteSequenceBlock : public pegtl::seq<AttributeOptionalSequence, NoteSequenceBlockWithoutAttributes>
{
};

class Phrase
    : public pegtl::seq<
        AttributeOptionalSequence,
        pegtl::if_must<
            pegtl_string_t("phrase"),
            Separator,
            pegtl::pad<Identifier, Separator>,
            NoteSequenceBlockWithoutAttributes
        >
    >
{
};

} // namespace Grammar

} // namespace Parser

} // namespace YAMML
