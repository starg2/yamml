
#pragma once

#include <string>

#include <boost/variant.hpp>

#include <ast/sourcelocation.hpp>

namespace YAMML
{

namespace AST
{

class UnsignedInteger final
{
public:
    unsigned long Value;
    SourceLocation Location;
};

class SignedInteger final
{
public:
    long Value;
    SourceLocation Location;
};

class Identifier final
{
public:
    std::string Value;
    SourceLocation Location;
};

class Literal final
{
public:
    boost::variant<long, double, std::string> Value;
    SourceLocation Location;
};

} // namespace AST

} // namespace YAMML
