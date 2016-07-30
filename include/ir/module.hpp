
#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <ir/block.hpp>
#include <ir/track.hpp>

namespace YAMML
{

namespace IR
{

class Module final
{
public:
    std::unordered_map<std::string, BlockReference> BlockNameMap;
    std::unordered_map<std::string, TrackBlockReference> TrackBlockNameMap;
    std::vector<Block> Blocks;
    std::vector<TrackBlock> TrackBlocks;
    std::string Name;
};

} // namespace IR

} // namespace YAMML
