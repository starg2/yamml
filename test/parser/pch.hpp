
#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include <boost/test/unit_test.hpp>
#include <boost/variant.hpp>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4244) // conversion from 'const std::streamsize' to 'std::size_t', possible loss of data
#pragma warning(disable: 4702) // unreachable code
#endif

#include <pegtl.hh>
#include <pegtl/trace.hh>

#ifdef _MSC_VER
#pragma warning(pop)
#endif
