
#pragma once

#include <memory>
#include <string>

#include <ast/attribute.hpp>

#include <irprocessor/attrproc.hpp>

namespace YAMML
{

namespace IRProcessor
{

class IAttributeProcessorFactory
{
public:
    virtual ~IAttributeProcessorFactory() = default;

    virtual std::unique_ptr<IAttributeProcessor> CreateProcessor(const AST::Attribute& ast) = 0;
    virtual std::string GetAttributeName() const = 0;
};

} // namespace IRProcessor

} // namespace YAMML
