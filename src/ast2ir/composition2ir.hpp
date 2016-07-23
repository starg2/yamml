
#pragma once

#include <boost/optional.hpp>

#include <ast/composition.hpp>
#include <compiler/nested.hpp>
#include <ir/module.hpp>

namespace YAMML
{

namespace AST2IR
{

class Composition2IRCompiler final : public Compiler::NestedCompilerBase
{
public:
    Composition2IRCompiler(Compiler::CompilerBase& parentCompiler, IR::Module& ir);

    bool Compile(const AST::Composition& ast, std::size_t index);

private:
    IR::Module& m_IR;
};

} // namespace AST2IR

} // namespace YAMML
