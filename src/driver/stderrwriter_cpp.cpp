
#ifndef _WIN32

#include <iostream>
#include <memory>
#include <string>

#include "stderrwriter.hpp"

namespace YAMML
{

namespace Driver
{

class CppStdErrWriter : public IStdErrWriter
{
public:
    virtual ~CppStdErrWriter() = default;

    virtual void Write(const std::string& str) override
    {
        std::cerr << str;
    }
};

std::unique_ptr<IStdErrWriter> CreateStdErrWriter()
{
    return std::make_unique<CppStdErrWriter>();
}

} // namespace Driver

} // namespace YAMML

#endif // !_WIN32
