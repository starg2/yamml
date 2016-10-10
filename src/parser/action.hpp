
#pragma once

#include <pegtl.hh>

namespace YAMML
{

namespace Parser
{

class AssignLocationAction
{
public:
    template<typename TInput, typename TState, typename... TCommonStates>
    static void apply(const TInput& in, TState& st, TCommonStates&...)
    {
        st.ASTNode.Location = {in.line(), in.byte_in_line()};
    }
};

} // namespace Parser

} // namespace YAMML
