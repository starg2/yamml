
#pragma once

#include <ir/event.hpp>

namespace YAMML
{

namespace IRProcessor
{

class IAttributeProcessor
{
public:
    virtual ~IAttributeProcessor() = default;

    virtual void TransformEvent(IR::Event& ev) = 0;
};

} // namespace IRProcessor

} // namespace YAMML
