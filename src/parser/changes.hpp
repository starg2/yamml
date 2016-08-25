
#pragma once

#include <type_traits>

#include <pegtl.hh>
#include <pegtl/contrib/changes.hh>

namespace YAMML
{

namespace Parser
{

// Based on pegtl/contrib/changes.hh, which is "Copyright (c) 2015 Dr. Colin Hirsch and Daniel Frey" (https://github.com/ColinH/PEGTL/)

template<typename TRule, typename TState, template<typename...> class TBase = pegtl::normal>
class ChangeState : public TBase<TRule>
{
public:
    template<
        pegtl::apply_mode TApplyMode,
        template<typename...> class TAction,
        template<typename...> class TControl,
        typename TInput,
        typename TParentState,
        typename... TCommonStates
    >
    static bool match(TInput& in, TParentState& parentState, TCommonStates&... commonStates)
    {
        pegtl::internal::state_disable_helper<TApplyMode, TState> childState;

        if (TBase<TRule>::template match<TApplyMode, TAction, TControl>(in, childState, commonStates...))
        {
            childState.success(parentState, commonStates...);
            return true;
        }
        else
        {
            return false;
        }
    }
};

template<typename TRule, typename TState, template<typename...> class TAction, template<typename...> class TBase = pegtl::normal>
class ChangeStateAndAction : public ChangeState<TRule, TState, pegtl::change_both_helper<TAction, TBase>::template change_action>
{
};

} // namespace Parser

} // namespace YAMML
