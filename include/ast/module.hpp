
#pragma once

#include <string>
#include <unordered_map>

#include <ast/composition.hpp>
#include <ast/phrase.hpp>

namespace YAMML
{

namespace AST
{

class Module final
{
public:
    void Add(const Composition& c)
    {
        m_Composition[c.Name] = c;
    }

    void Add(const Phrase& ph)
    {
        m_Phrases[ph.Name] = ph;
    }

private:
    std::string m_Name;
    std::unordered_map<std::string, Phrase> m_Phrases;
    std::unordered_map<std::string, Composition> m_Composition;
};

} // namespace AST

} // namespace YAMML
