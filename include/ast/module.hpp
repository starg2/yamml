
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

private:
    std::string m_Name;
    std::unordered_map<std::string, Phrase> m_Phrases;
    std::unordered_map<std::string, Composition> m_Composition;
};

} // namespace AST

} // namespace YAMML
