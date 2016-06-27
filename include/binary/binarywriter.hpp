
#pragma once

#include <cstdint>

#include <algorithm>
#include <iterator>
#include <string>
#include <vector>

namespace YAMML
{

namespace Binary
{

class BinaryWriter
{
public:
    virtual ~BinaryWriter() = default;

    std::vector<std::uint8_t>& GetBuffer()
    {
        return m_Buffer;
    }

    const std::vector<std::uint8_t>& GetBuffer() const
    {
        return m_Buffer;
    }

    void Clear()
    {
        m_Buffer.clear();
    }

    void AppendUInt8(std::uint8_t n)
    {
        m_Buffer.push_back(n);
    }

    void AppendUInt16BE(std::uint16_t n)
    {
        m_Buffer.push_back(n >> 8);
        m_Buffer.push_back(n & 0xFF);
    }

    void AppendUInt32BE(std::uint32_t n)
    {
        m_Buffer.push_back(n >> 24);
        m_Buffer.push_back((n >> 16) & 0xFF);
        m_Buffer.push_back((n >> 8) & 0xFF);
        m_Buffer.push_back(n & 0xFF);
    }

    void AppendString(const char* pString)
    {
        std::string str = pString;
        AppendBuffer(str.begin(), str.end());
    }

    template<typename T>
    void AppendBuffer(const T& container)
    {
        using std::begin;
        using std::end;

        AppendBuffer(begin(container), end(container));
    }

    template<typename TIterator>
    void AppendBuffer(TIterator itBegin, TIterator itEnd)
    {
        std::copy(itBegin, itEnd, std::back_inserter(m_Buffer));
    }

private:
    std::vector<std::uint8_t> m_Buffer;
};

} // namespace Binary

} // namespace YAMML
