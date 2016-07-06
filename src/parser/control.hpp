
#pragma once

#include <pegtl.hh>
#include <pegtl/contrib/changes.hh>

#include "error_control.hpp"

namespace YAMML
{

namespace Parser
{

template<typename TRule>
class Control : public ErrorControl<TRule>
{
};

} // namespace Parser

} // namespace YAMML
