
#ifdef _WIN32

#include <cstdint>

#include <algorithm>
#include <array>
#include <string>
#include <utility>
#include <vector>

#include <windows.h>

#include "cconv.hpp"
#include "file.hpp"

namespace YAMML
{

namespace Driver
{

class File final
{
public:
    enum class OpenMode
    {
        Read,
        Write,
        Append
    };

    File() noexcept : m_hFile(INVALID_HANDLE_VALUE)
    {
    }

    File(const std::wstring& filePath, OpenMode mode) : File()
    {
        Open(filePath, mode);
    }

    File(const File&) = delete;
    File& operator=(const File&) = delete;

    File(File&& rhs) noexcept : File()
    {
        Swap(rhs);
    }

    File& operator=(File&& rhs) noexcept
    {
        Close();
        Swap(rhs);
        return *this;
    }

    ~File()
    {
        Close();
    }

    void Swap(File& rhs)
    {
        using std::swap;

        swap(m_hFile, rhs.m_hFile);
    }

    bool IsOpened() const
    {
        return m_hFile != INVALID_HANDLE_VALUE;
    }

    explicit operator bool() const
    {
        return IsOpened();
    }

    void Open(const std::wstring& filePath, OpenMode mode)
    {
        if (IsOpened())
        {
            throw std::logic_error("File is already opened.");
        }

        m_hFile = ::CreateFileW(
            FixPath(filePath).c_str(),
            GENERIC_READ | (mode == OpenMode::Read ? 0 : GENERIC_WRITE),
            (mode == OpenMode::Read ? FILE_SHARE_READ : 0),
            nullptr,
            (mode == OpenMode::Read ? OPEN_EXISTING : mode == OpenMode::Write ? CREATE_ALWAYS : OPEN_ALWAYS),
            FILE_ATTRIBUTE_NORMAL,
            nullptr
        );

        if (!IsOpened())
        {
            throw FileOpenException(W2UTF8(filePath));
        }
    }

    void Close()
    {
        if (IsOpened())
        {
            ::CloseHandle(m_hFile);
            m_hFile = INVALID_HANDLE_VALUE;
        }
    }

    std::vector<std::uint8_t> Read()
    {
        std::vector<std::uint8_t> ret;
        std::array<std::uint8_t, 4096> buffer;
        DWORD bytes = 0;

        while (::ReadFile(m_hFile, buffer.data(), static_cast<DWORD>(buffer.size()), &bytes, nullptr) && bytes > 0)
        {
            ret.insert(ret.end(), buffer.begin(), buffer.begin() + bytes);
        }

        return ret;
    }

    void Write(const std::vector<std::uint8_t>& c)
    {
        if (!IsOpened())
        {
            throw std::logic_error("File is not opened yet.");
        }

        DWORD bytesWritten;

        if (!::WriteFile(m_hFile, c.data(), static_cast<DWORD>(c.size()), &bytesWritten, nullptr) || c.size() != bytesWritten)
        {
            throw IOException();
        }
    }

private:
    static std::wstring FixPath(const std::wstring& oldFilePath)
    {
        if (oldFilePath.length() >= MAX_PATH && oldFilePath.substr(0, 4) != LR"(\\?\)")
        {
            if (oldFilePath.substr(0, 2) == LR"(\\)") // UNC
            {
                return LR"(\\?\UNC)" + oldFilePath.substr(1);
            }
            else
            {
                return LR"(\\?\)" + oldFilePath;
            }
        }
        else
        {
            return oldFilePath;
        }
    }

    HANDLE m_hFile;
};

std::string ReadTextFile(const std::string& filePath)
{
    File f(UTF82W(filePath), File::OpenMode::Read);
    auto buffer = f.Read();

    if (buffer.size() >= 3 && buffer[0] == 0xEF && buffer[1] == 0xBB && buffer[2] == 0xBF)  // UTF-8 w/ BOM
    {
        return std::string(buffer.begin() + 3, buffer.end());
    }
    else if (buffer.size() >= 2 && buffer[0] == 0xFF && buffer[1] == 0xFE)  // UTF-16LE
    {
        std::wstring wstr((buffer.size() - 1) / 2, L'\0');
        std::copy(buffer.begin() + 2, buffer.end(), reinterpret_cast<char*>(&wstr[0]));
        return W2UTF8(wstr);
    }
    else  // UTF-8 w/o BOM
    {
        return std::string(buffer.begin(), buffer.end());
    }
}

void WriteBinaryFile(const std::string& filePath, const std::vector<std::uint8_t>& buffer)
{
    File f(UTF82W(filePath), File::OpenMode::Write);
    f.Write(buffer);
}

} // namespace Driver

} // namespace YAMML

#endif // _WIN32
