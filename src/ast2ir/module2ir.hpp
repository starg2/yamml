
#pragma once

#include <boost/optional.hpp>

#include <ast/module.hpp>
#include <compiler/nested.hpp>
#include <ir/module.hpp>

namespace YAMML
{

namespace AST2IR
{

class Module2IRCompiler final : public Compiler::NestedCompilerBase
{
public:
    using NestedCompilerBase::NestedCompilerBase;

    boost::optional<IR::Module> Compile(const AST::Module& ast);
};

} // namespace AST2IR

} // namespace YAMML
