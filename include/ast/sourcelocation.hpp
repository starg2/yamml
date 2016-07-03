
#pragma once

#include <cstddef>

namespace YAMML
{

namespace AST
{

struct SourceLocation
{
    std::size_t Line;
    std::size_t Column;
};

} // namespace AST

} // namespace YAMML
