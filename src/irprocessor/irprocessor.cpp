
#include <irprocessor/irprocessor.hpp>

namespace YAMML
{

namespace IRProcessor
{

bool IRCompiler::Compile()
{
    return true;
}

const IR::Module& IRCompiler::GetIR() const
{
    return m_IR;
}

} // namespace IRProcessor

} // namespace YAMML
