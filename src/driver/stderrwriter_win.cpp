
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
    explicit WinConsoleStdErrWriter(HANDLE hStdErr) : m_hStdErr{hStdErr}
    {
    }

    virtual ~WinConsoleStdErrWriter() = default;

    virtual void Write(const std::string& str) override
    {
        UTF82W wstr(str);
        DWORD charsWritten;
        ::WriteConsoleW(m_hStdErr, wstr.GetCString(), static_cast<DWORD>(wstr.GetString().length()), &charsWritten, nullptr);
    }

private:
    HANDLE m_hStdErr;
};

class WinFileStdErrWriter : public IStdErrWriter
{
public:
    explicit WinFileStdErrWriter(HANDLE hStdErr) : m_hStdErr{hStdErr}
    {
    }

    virtual ~WinFileStdErrWriter() = default;

    virtual void Write(const std::string& str) override
    {
        DWORD bytesWritten;
        ::WriteFile(m_hStdErr, str.data(), static_cast<DWORD>(str.size()), &bytesWritten, nullptr);
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
