
#pragma once

#include <pegtl.hh>

namespace YAMML
{

namespace Parser
{

class AssignLocationAction
{
public:
    template<typename TState, typename... TCommonStates>
    static void apply(const pegtl::input& in, TState& st, TCommonStates&...)
    {
        st.ASTNode.Location = {in.line(), in.column()};
    }
};

} // namespace Parser

} // namespace YAMML
