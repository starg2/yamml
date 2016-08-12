
#pragma once

#include <cstdint>

#include <stdexcept>
#include <string>
#include <vector>

namespace YAMML
{

namespace Driver
{

class IOException : public std::runtime_error
{
public:
    IOException() : runtime_error("IOException")
    {
    }
};


std::string ReadTextFile(const std::string& filePath);
void WriteBinaryFile(const std::string& filePath, const std::vector<std::uint8_t>& buffer);

} // namespace Driver

} // namespace YAMML
