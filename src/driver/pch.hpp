
#pragma once

#if defined(_MSC_VER) && defined(_WIN32) && defined(_DEBUG)

#include <crtdbg.h>

#endif // _WIN32

#include <cctype>
#include <cstdint>

#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <boost/optional.hpp>
#include <boost/program_options.hpp>

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

