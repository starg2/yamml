
#pragma once

#include <string>
#include <vector>

#include <ast/composition.hpp>
#include <ast/phrase.hpp>

namespace YAMML
{

namespace AST
{

class Module final
{
public:
    std::string Name;
    std::vector<Phrase> Phrases;
    std::vector<Composition> Compositions;
};

} // namespace AST

} // namespace YAMML
