
#pragma once

#include <boost/optional.hpp>

#include <ast/module.hpp>
#include <compiler/base.hpp>
#include <ir/module.hpp>

namespace YAMML
{

namespace AST2IR
{

class AST2IRCompiler final : public Compiler::CompilerBase
{
public:
    bool Compile(const AST::Module& mod);

    boost::optional<IR::Module>& GetIR();
    const boost::optional<IR::Module>& GetIR() const;

private:
    boost::optional<IR::Module> m_IR;
};

} // namespace AST2IR

} // namespace YAMML
