
#pragma once

#include <pegtl.hh>

#include "parser_skips.hpp"

namespace YAMML
{

namespace Parser
{

namespace Grammar
{

class UnsignedIntegerBase
    : public pegtl::sor<
        pegtl::if_must<pegtl::one<'0'>, pegtl::not_at<pegtl::digit>>,
        pegtl::seq<pegtl::range<'1', '9'>, pegtl::star<pegtl::digit>>
    >
{
};

class UnsignedInteger : public UnsignedIntegerBase
{
};

class SignedInteger : public pegtl::seq<pegtl::opt<pegtl::one<'+', '-'>>, UnsignedIntegerBase>
{
};

// http://en.cppreference.com/w/cpp/language/identifiers#Unicode_characters_in_identifiers
class UnicodeIdentifierChar
    : public pegtl::sor<
        pegtl::utf8::one<0x00A8, 0x00AA, 0x00AD, 0x00AF, 0x2054>,
        pegtl::utf8::ranges<
            0x00B2, 0x00B5,
            0x00B7, 0x00BA,
            0x00BC, 0x00BE,
            0x00C0, 0x00D6,
            0x00D8, 0x00F6,
            0x00F8, 0x167F,
            0x1681, 0x180D,
            0x180F, 0x1FFF,
            0x200B, 0x200D,
            0x202A, 0x202E,
            0x203F, 0x2040,
            0x2060, 0x218F,
            0x2460, 0x24FF,
            0x2776, 0x2793,
            0x2C00, 0x2DFF,
            0x2E80, 0x2FFF,
            0x3004, 0x3007,
            0x3021, 0x302F,
            0x3031, 0xD7FF,
            0xF900, 0xFD3D,
            0xFD40, 0xFDCF,
            0xFDF0, 0xFE44,
            0xFE47, 0xFFFD,
            0x10000, 0x1FFFD,
            0x20000, 0x2FFFD,
            0x30000, 0x3FFFD,
            0x40000, 0x4FFFD,
            0x50000, 0x5FFFD,
            0x60000, 0x6FFFD,
            0x70000, 0x7FFFD,
            0x80000, 0x8FFFD,
            0x90000, 0x9FFFD,
            0xA0000, 0xAFFFD,
            0xB0000, 0xBFFFD,
            0xC0000, 0xCFFFD,
            0xD0000, 0xDFFFD,
            0xE0000, 0xEFFFD
        >
    >
{
};

class IdentifierChar : public pegtl::sor<pegtl::alnum, pegtl::one<'_'>, UnicodeIdentifierChar>
{
};

class IdentifierCharFirst
    : public pegtl::seq<
        pegtl::not_at<pegtl::digit>,
        pegtl::not_at<
            pegtl::utf8::ranges<
                0x0300, 0x036F,
                0x1DC0, 0x1DFF,
                0x20D0, 0x20FF,
                0xFE20, 0xFE2F
            >
        >,
        IdentifierChar
    >
{
};

class Identifier : public pegtl::seq<IdentifierCharFirst, pegtl::star<IdentifierChar>>
{
};

// https://github.com/ColinH/PEGTL/blob/master/examples/unescape.cc
class EscapeX2 : public pegtl::seq<pegtl::one<'x'>, pegtl::rep<2, pegtl::must<pegtl::xdigit>>>
{
};

class EscapeU4 : public pegtl::seq<pegtl::one<'u'>, pegtl::rep<4, pegtl::must<pegtl::xdigit>>>
{
};

class EscapeU8 : public pegtl::seq<pegtl::one<'U'>, pegtl::rep<8, pegtl::must<pegtl::xdigit>>>
{
};

class EscapeC : public pegtl::one<'\'', '"', '?', '\\', 'a', 'b', 'f', 'n', 'r', 't', 'v'>
{
};

class Escape : public pegtl::sor<EscapeX2, EscapeU4, EscapeU8, EscapeC>
{
};

class AnyChar : public pegtl::utf8::range<0x20, 0x10FFFF>
{
};

class CharcterOrEscapeSequence : public pegtl::if_must_else<pegtl::one<'\\'>, Escape, AnyChar>
{
};

template<char TQuoteChar>
class QuotedStringLiteral
    : public pegtl::if_must<
        pegtl::one<TQuoteChar>,
        pegtl::until<pegtl::one<TQuoteChar>, CharcterOrEscapeSequence>
    >
{
};

class DoubleQuotedStringLiteral : public QuotedStringLiteral<'"'>
{
};

class SingleQuotedStringLiteral : public QuotedStringLiteral<'\''>
{
};

class Value : public pegtl::sor<SignedInteger, Identifier, DoubleQuotedStringLiteral, SingleQuotedStringLiteral>
{
};

} // namespace Grammar

} // namespace Parser

} // namespace YAMML
