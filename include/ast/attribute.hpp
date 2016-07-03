
#pragma once

#include <string>
#include <vector>

#include <ast/literal.hpp>
#include <ast/sourcelocation.hpp>

namespace YAMML
{

namespace AST
{

class Attribute final
{
public:
    std::string Name;
    std::vector<Literal> Arguments;
    SourceLocation Location;
};

} // namespace AST

} // namespace YAMML
