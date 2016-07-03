
#pragma once

#include <string>

#include <boost/variant.hpp>

#include <ast/sourcelocation.hpp>

namespace YAMML
{

namespace AST
{

class Literal final
{
public:
    boost::variant<int, double, std::string> Value;
    SourceLocation Location;
};

} // namespace AST

} // namespace YAMML
