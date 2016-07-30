
#pragma once

#include <vector>

#include <boost/variant.hpp>

#include <ast/attribute.hpp>
#include <ast/composition.hpp>
#include <ir/block.hpp>

namespace YAMML
{

namespace IR
{

class TrackBlockReference final
{
public:
    std::size_t ID;
};

class Track final
{
public:
    int Number;
    std::vector<IR::BlockReference> Blocks;
};

class TrackList final
{
public:
    std::vector<Track> Tracks;
};

class TrackBlock final
{
public:
    using BlockType = boost::variant<TrackList, AST::Command>;

    std::vector<AST::Attribute> Attributes;
    std::vector<BlockType> Blocks;
};

} // namespace IR

} // namespace YAMML
