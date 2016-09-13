
#pragma once

#include <deque>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <boost/variant.hpp>

#include <ast/attribute.hpp>
#include <compiler/base.hpp>
#include <ir/module.hpp>
#include <irprocessor/attrprocfactory.hpp>

namespace YAMML
{

namespace IRProcessor
{

class IRCompiler : public Compiler::CompilerBase, public boost::static_visitor<>
{
public:
    template<typename T>
    IRCompiler(const IR::Module& ir, T func) : CompilerBase(func), m_IR(ir)
    {
    }

    IRCompiler(const IRCompiler&) = delete;
    IRCompiler& operator=(const IRCompiler&) = delete;

    bool Compile();

    std::string GetSourceName() const;
    const IR::Module& GetIR() const;

    void operator()(IR::TrackList& trackList);
    void operator()(AST::Command& command);

    void operator()(IR::Event& ev);
    void operator()(IR::BlockReference& blockRef);

    void CompileBlock(IR::BlockReference blockRef);

private:
    void CompileTrackBlock(std::size_t index);

    void AddAttributeProcessorFactory(std::unique_ptr<IAttributeProcessorFactory> pAttributeProcessorFactory);
    void InitializeAttributeProcessorFactories();

    void RegisterAttributeProcessors(const std::vector<AST::Attribute>& attributes);

    IR::BlockReference DuplicateBlock(IR::BlockReference blockRef);

    IR::Module m_IR;
    std::unordered_map<std::string, std::unique_ptr<IAttributeProcessorFactory>> m_AttributeProcessorFactoryMap;
    std::deque<std::vector<std::unique_ptr<IAttributeProcessor>>> m_AttributeProcessorStack;
};

} // namespace IRProcessor

} // namespace YAMML
