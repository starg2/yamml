
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

    bool Compile();

    const IR::Module& GetIR() const;

private:
    IR::Module m_IR;
};

} // namespace IRProcessor

} // namespace YAMML
