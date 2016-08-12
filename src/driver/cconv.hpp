
#pragma once

#ifdef _WIN32

#include <string>

namespace YAMML
{

namespace Driver
{

class UTF82W final
{
public:
    explicit UTF82W(const std::string& str);

    const std::wstring& GetString() const;
    const wchar_t* GetCString() const;
    operator const std::wstring&() const;
    operator const wchar_t*() const;

private:
    std::wstring m_WString;
};

class W2UTF8 final
{
public:
    explicit W2UTF8(const std::wstring& wstr);

    const std::string& GetString() const;
    const char* GetCString() const;
    operator const std::string&() const;
    operator const char*() const;

private:
    std::string m_String;
};

} // namespace Driver

} // namespace YAMML

#endif // _WIN32

