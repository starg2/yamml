
#pragma once

namespace YAMML
{

namespace Common
{

template<typename T>
class AutoPop final
{
public:
    explicit AutoPop(T& c) : m_Container(c)
    {
    }

    AutoPop(const AutoPop&) = delete;
    AutoPop(AutoPop&&) = delete;
    AutoPop& operator=(const AutoPop&) = delete;
    AutoPop& operator=(AutoPop&&) = delete;

    ~AutoPop()
    {
        m_Container.pop_back();
    }

private:
    T& m_Container;
};

} // namespace Common

} // namespace YAMML
