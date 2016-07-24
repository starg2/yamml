
#pragma once

#include <vector>

namespace YAMML
{

namespace AST2IR
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

template<typename T>
auto Concat(const T& container)
{
    std::vector<typename T::value_type::value_type> v;

    for (auto&& i : container)
    {
        v.insert(v.end(), i.begin(), i.end());
    }

    return v;
}


} // namespace AST2IR

} // namespace YAMML
