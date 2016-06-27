
#pragma once

#include <exception>

namespace YAMML
{

namespace Exceptions
{

class Exception : public std::exception
{
public:
    using exception::exception;
};

} // namespace Exceptions

} // namespace YAMML
