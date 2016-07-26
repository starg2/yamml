
#pragma once

#include <string>
#include <vector>

#include <boost/variant.hpp>

#include <ast/attribute.hpp>

#include <ir/event.hpp>

namespace YAMML
{

namespace IR
{

class BlockReference final
{
public:
    std::size_t ID;
};

class Block final
{
public:
    using EventType = boost::variant<Event, BlockReference>;

    std::vector<AST::Attribute> Attributes;
    std::vector<EventType> Events;
};

} // namespace IR

} // namespace YAMML
