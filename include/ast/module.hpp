
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
        Compositions[c.Name] = c;
    }

    void Add(const Phrase& ph)
    {
        Phrases[ph.Name] = ph;
    }

    std::string Name;
    std::unordered_map<std::string, Phrase> Phrases;
    std::unordered_map<std::string, Composition> Compositions;
};

} // namespace AST

} // namespace YAMML
