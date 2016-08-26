
#pragma once

#include <compiler/base.hpp>
#include <ir/module.hpp>

namespace YAMML
{

namespace IRProcessor
{

class IRCompiler : public Compiler::CompilerBase
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

private:
    void CompileTrackBlock(std::size_t index);

    IR::Module m_IR;
};

} // namespace IRProcessor

} // namespace YAMML
