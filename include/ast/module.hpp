
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
    // returns false if an item with the same name already exists
    bool TryAdd(const Composition& c)
    {
        if (Compositions.find(c.Name) == Compositions.end())
        {
            Compositions[c.Name] = c;
            return true;
        }
        else
        {
            return false;
        }
    }

    bool TryAdd(const Phrase& ph)
    {
        if (Phrases.find(ph.Name) == Phrases.end())
        {
            Phrases[ph.Name] = ph;
            return true;
        }
        else
        {
            return false;
        }
    }

    std::string Name;
    std::unordered_map<std::string, Phrase> Phrases;
    std::unordered_map<std::string, Composition> Compositions;
};

} // namespace AST

} // namespace YAMML
