
#pragma once

#include <memory>
#include <string>

namespace YAMML
{

namespace Driver
{

class IStdErrWriter
{
public:
    virtual ~IStdErrWriter() = default;

    virtual void Write(const std::string& str) = 0;
    
    void WriteLine(const std::string& str)
    {
        Write(str + '\n');
    }
};

std::unique_ptr<IStdErrWriter> CreateStdErrWriter();

} // namespace Driver

} // namespace YAMML
