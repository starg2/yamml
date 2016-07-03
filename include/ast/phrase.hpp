
#pragma once

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

class NoteName final
{
public:
    char Name;
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
    int Number;
    SourceLocation Location;
};

class SimpleDurationModifier final
{
public:
    int Number;
    SourceLocation Location;
};

class SimpleDurationWithModifier final
{
public:
    SimpleDuration Base;
    boost::optional<SimpleDurationModifier> Modifier;
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
    NoteNumber Note;
    boost::optional<DurationSet> Duration;
    SourceLocation Location;
};

class NoteSequence;

class NoteRepeatExpression final
{
public:
    std::size_t Count;
    std::vector<boost::variant<NoteAndDuration, boost::recursive_wrapper<NoteSequence>>> Notes;
    SourceLocation Location;
};

class NoteRepeatEachExpression final
{
public:
    std::size_t Count;
    std::vector<NoteRepeatExpression> Notes;
    SourceLocation Location;
};

class NoteAndExpression final
{
public:
    std::vector<NoteRepeatEachExpression> Notes;
    SourceLocation Location;
};

class NoteSequence final
{
public:
    std::vector<NoteAndExpression> Notes;
    SourceLocation Location;
};

class NoteSequenceBlock final
{
public:
    std::vector<Attribute> Attributes;
    std::vector<NoteSequence> Sequences;
    SourceLocation Location;
};

class Phrase final
{
public:
    std::string Name;
    std::vector<Attribute> Attributes;
    std::vector<boost::variant<NoteSequence, NoteSequenceBlock>> Statements;
    SourceLocation Location;
};

} // namespace AST

} // namespace YAMML
