
#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <ir/block.hpp>

namespace YAMML
{

namespace IR
{

class Module final
{
public:
    std::unordered_map<std::string, std::size_t> BlockNameMap;
    std::vector<Block> Blocks;
    std::string Name;
};

} // namespace IR

} // namespace YAMML
