
#pragma once

#include <memory>
#include <string>
#include <vector>

#include <boost/optional.hpp>
#include <boost/variant.hpp>

#include <ast/attribute.hpp>
#include <ast/sourcelocation.hpp>

namespace YAMML
{

namespace AST
{

class Rest final
{
public:
    SourceLocation Location;
};

class NoteName final
{
public:
    char Name;
    int Minor;
    SourceLocation Location;
};

class NoteOctave final
{
public:
    int Value;
    SourceLocation Location;
};

class NoteNumber final
{
public:
    NoteName Name;
    boost::optional<NoteOctave> Octave;
    SourceLocation Location;
};

class SimpleChord final
{
public:
    std::vector<NoteNumber> Notes;
    SourceLocation Location;
};

class SimpleDuration final
{
public:
    unsigned long Number;
    SourceLocation Location;
};

class SimpleDurationModifier final
{
public:
    unsigned long Number;
    SourceLocation Location;
};

class SimpleDurationModifierDots final
{
public:
    unsigned long Count;
    SourceLocation Location;
};

class SimpleDurationWithModifier final
{
public:
    SimpleDuration Base;
    boost::optional<boost::variant<SimpleDurationModifier, SimpleDurationModifierDots>> Modifier;
    SourceLocation Location;
};

class DurationSet final
{
public:
    std::vector<SimpleDurationWithModifier> Durations;
    SourceLocation Location;
};

class NoteAndDuration final
{
public:
    boost::variant<Rest, NoteNumber, SimpleChord> Note;
    boost::optional<boost::variant<SimpleDurationWithModifier, DurationSet>> Duration;
    SourceLocation Location;
};

class NoteSequence;

class NoteRepeatExpression final
{
public:
    std::size_t Count;
    std::vector<boost::recursive_wrapper<NoteSequence>> Notes;
    SourceLocation Location;
};

class NoteRepeatEachExpression final
{
public:
    std::size_t Count;
    std::vector<boost::recursive_wrapper<NoteSequence>> Notes;
    SourceLocation Location;
};

class NoteAndExpression final
{
public:
    std::vector<boost::variant<NoteAndDuration, NoteRepeatEachExpression, NoteRepeatExpression, boost::recursive_wrapper<NoteSequence>>> Notes;
    SourceLocation Location;
};

class NoteSequence final
{
public:
    std::vector<NoteAndExpression> Notes;
    SourceLocation Location;
};

class NoteSequenceStatement final
{
public:
    std::vector<Attribute> Attributes;
    boost::optional<NoteSequence> NoteSeq;
    SourceLocation Location;
};

class NoteSequenceBlock;

class NoteSequenceBlockWithoutAttributes final
{
public:
    std::vector<boost::variant<NoteSequenceStatement, boost::recursive_wrapper<NoteSequenceBlock>>> Sequences;
    SourceLocation Location;
};

class NoteSequenceBlock final
{
public:
    std::vector<Attribute> Attributes;
    NoteSequenceBlockWithoutAttributes Block;
    SourceLocation Location;
};

class Phrase final
{
public:
    std::string Name;
    std::vector<Attribute> Attributes;
    NoteSequenceBlockWithoutAttributes Block;
    SourceLocation Location;
};

} // namespace AST

} // namespace YAMML
