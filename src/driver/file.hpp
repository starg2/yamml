
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
    using runtime_error::runtime_error;

    IOException() : runtime_error("IOException")
    {
    }
};

class FileOpenException : public IOException
{
public:
    FileOpenException(const std::string filePath)
        : IOException("FileOpenException: Unable to open file '" + filePath + "'"), FilePath(filePath)
    {
    }

    std::string FilePath;
};

std::string ReadTextFile(const std::string& filePath);
void WriteBinaryFile(const std::string& filePath, const std::vector<std::uint8_t>& buffer);

} // namespace Driver

} // namespace YAMML
