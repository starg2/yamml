
#ifdef _WIN32

#include <string>

#include <windows.h>

#include "cconv.hpp"

namespace YAMML
{

namespace Driver
{

UTF82W::UTF82W(const std::string& str)
{
    int requiredSize = ::MultiByteToWideChar(
        CP_UTF8,
        0,
        str.data(),
        static_cast<int>(str.size()),
        nullptr,
        0
    );

    m_WString.assign(requiredSize + 1, L'\0');

    ::MultiByteToWideChar(
        CP_UTF8,
        0,
        str.data(),
        static_cast<int>(str.size()),
        &m_WString[0],
        requiredSize
    );
}

const std::wstring& UTF82W::GetString() const
{
    return m_WString;
}

const wchar_t* UTF82W::GetCString() const
{
    return m_WString.c_str();
}

UTF82W::operator const std::wstring&() const
{
    return GetString();
}

UTF82W::operator const wchar_t*() const
{
    return GetCString();
}

W2UTF8::W2UTF8(const std::wstring& wstr)
{
    int requiredSize = ::WideCharToMultiByte(
        CP_UTF8,
        0,
        wstr.data(),
        static_cast<int>(wstr.size()),
        nullptr,
        0,
        nullptr,
        nullptr
    );

    m_String.assign(requiredSize + 1, '\0');

    ::WideCharToMultiByte(
        CP_UTF8,
        0,
        wstr.data(),
        static_cast<int>(wstr.size()),
        &m_String[0],
        requiredSize,
        nullptr,
        nullptr
    );
}

const std::string& W2UTF8::GetString() const
{
    return m_String;
}

const char* W2UTF8::GetCString() const
{
    return m_String.c_str();
}

W2UTF8::operator const std::string&() const
{
    return GetString();
}

W2UTF8::operator const char*() const
{
    return GetCString();
}

} // namespace Driver

} // namespace YAMML

#endif // _WIN32
