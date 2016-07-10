
#pragma once

#include <string>
#include <vector>

#include <boost/optional.hpp>

#include <ast/literal.hpp>
#include <ast/sourcelocation.hpp>

namespace YAMML
{

namespace AST
{

class AttributeArgument final
{
public:
    boost::optional<std::string> Name;
    Literal Value;
    SourceLocation Location;
};

class Attribute final
{
public:
    std::string Name;
    std::vector<AttributeArgument> Arguments;
    SourceLocation Location;
};

} // namespace AST

} // namespace YAMML
