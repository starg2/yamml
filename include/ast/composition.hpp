
#pragma once

#include <string>
#include <vector>

#include <boost/variant.hpp>

#include <ast/attribute.hpp>
#include <ast/sourcelocation.hpp>

namespace YAMML
{

namespace AST
{

class Command final
{
public:
    std::string Name;
    std::vector<Literal> Arguments;
    SourceLocation Location;
};

class TrackItem final
{
public:
    std::vector<Attribute> Attributes;
    std::string PhraseName;
    SourceLocation Location;
};

class TrackBlock final
{
public:
    int TrackNumber;
    std::vector<TrackItem> Items;
    SourceLocation Location;
};

class TrackListBlock final
{
public:
    std::vector<Attribute> Attributes;
    std::vector<TrackBlock> Tracks;
    SourceLocation Location;
};

class Composition final
{
public:
    std::string Name;
    std::vector<Attribute> Attributes;
    std::vector<boost::variant<Command, Attribute, TrackListBlock>> Statements;
    SourceLocation Location;
};

} // namespace AST

} // namespace YAMML
