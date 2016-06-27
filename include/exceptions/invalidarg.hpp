
#pragma once

#include <exceptions/exception.hpp>

namespace YAMML
{

namespace Exceptions
{

class InvalidArgumentException : public Exception
{
public:
    using Exception::Exception;
};

} // namespace Exceptions

} // namespace YAMML
