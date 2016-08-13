
#ifndef _WIN32

#include <cstdint>

#include <fstream>
#include <ios>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "file.hpp"

namespace YAMML
{

namespace Driver
{

std::string ReadTextFile(const std::string& filePath)
{
    std::ifstream ifs(filePath);

    if (!ifs)
    {
        throw FileOpenException(filePath);
    }

    std::ostringstream oss;
    oss << ifs.rdbuf();

    if (!oss)
    {
        throw IOException();
    }

    return oss.str();
}

void WriteBinaryFile(const std::string& filePath, const std::vector<std::uint8_t>& buffer)
{
    std::ofstream ofs(filePath, std::ios_base::binary);

    if (!ofs)
    {
        throw FileOpenException(filePath);
    }

    ofs.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());

    if (ofs.bad())
    {
        throw IOException();
    }
}

} // namespace Driver

} // namespace YAMML

#endif // _WIN32
