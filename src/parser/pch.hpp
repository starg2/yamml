
#pragma once

#include <algorithm>
#include <functional>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <boost/optional.hpp>
#include <boost/variant.hpp>

#include <pegtl.hh>
#include <pegtl/contrib/changes.hh>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4715)  // not all control paths return a value
#endif

#include <pegtl/contrib/unescape.hh>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include <pegtl/internal/demangle.hh>

