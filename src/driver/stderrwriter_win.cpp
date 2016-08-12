
#ifdef _WIN32

#include <memory>
#include <string>

#include <windows.h>

#include "cconv.hpp"
#include "stderrwriter.hpp"

namespace YAMML
{

namespace Driver
{

class WinConsoleStdErrWriter : public IStdErrWriter
{
public:
    WinConsoleStdErrWriter(HANDLE hStdErr) : m_hStdErr{hStdErr}
    {
    }

    virtual ~WinConsoleStdErrWriter() = default;

    virtual void Write(const std::string& str) override
    {
        UTF82W wstr(str);
        ::WriteConsoleW(m_hStdErr, wstr.GetCString(), wstr.GetString().length(), nullptr, nullptr);
    }

private:
    HANDLE m_hStdErr;
};

class WinFileStdErrWriter : public IStdErrWriter
{
public:
    WinFileStdErrWriter(HANDLE hStdErr) : m_hStdErr{hStdErr}
    {
    }

    virtual ~WinFileStdErrWriter() = default;

    virtual void Write(const std::string& str) override
    {
        ::WriteFile(m_hStdErr, str.data(), str.size(), nullptr, nullptr);
    }

private:
    HANDLE m_hStdErr;
};

std::unique_ptr<IStdErrWriter> CreateStdErrWriter()
{
    HANDLE hStdErr = ::GetStdHandle(STD_ERROR_HANDLE);
    DWORD mode;

    if (::GetConsoleMode(hStdErr, &mode))
    {
        return std::make_unique<WinConsoleStdErrWriter>(hStdErr);
    }
    else
    {
        return std::make_unique<WinFileStdErrWriter>(hStdErr);
    }
}

} // namespace Driver

} // namespace YAMML

#endif // _WIN32
