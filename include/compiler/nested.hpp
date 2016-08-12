
#pragma once

#include <compiler/base.hpp>

namespace YAMML
{

namespace Compiler
{

class NestedCompilerBase : public CompilerBase
{
public:
    explicit NestedCompilerBase(CompilerBase& parentCompiler)
        : CompilerBase([this] (auto&& x) { m_ParentCompiler.AddMessage(x); return true; }), m_ParentCompiler(parentCompiler)
    {
    }

    CompilerBase& GetParentCompiler()
    {
        return m_ParentCompiler;
    }

private:
    CompilerBase& m_ParentCompiler;
};

} // namespace Compiler

} // namespace YAMML
