
#pragma once

#include <cctype>

#include <string>
#include <vector>

namespace YAMML
{

namespace Driver
{

std::string FormatText(std::string format, const std::vector<std::string>& args)
{
    for (std::size_t i = format.find('{'); i != format.npos && i + 2 < format.length(); i = format.find('{', i))
    {
        std::size_t close = format.find('}', i + 2);

        if (close == format.npos)
        {
            break;
        }
        else if (close - i > 1 && std::isdigit(format.at(i + 1)))
        {
            auto arg = args.at(std::stoul(format.substr(i + 1, close - i - 1)));
            format.replace(i, close - i + 1, arg);
            i += arg.length();
        }
        else
        {
            format.erase(close, 1);
            format.erase(i, 1);
            i = close - 1;
        }
    }

    return format;
}

} // namespace Driver

} // namespace YAMML
