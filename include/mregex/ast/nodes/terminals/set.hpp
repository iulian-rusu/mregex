#ifndef MREGEX_NODES_SET_HPP
#define MREGEX_NODES_SET_HPP

#include <mregex/ast/nodes/terminals/trivially_matchable.hpp>
#include <mregex/ast/match_result.hpp>
#include <mregex/ast/traits.hpp>

namespace meta::ast
{
    template<typename First, typename... Rest>
    struct set : trivially_matchable<set<First, Rest ...>>
    {
        static_assert(are_trivially_matchable<First, Rest ...>, "only trivially matchable AST nodes can form a set");

        template<std::forward_iterator Iter, typename Context>
        static constexpr bool match_one(Iter current, Context &ctx) noexcept
        {
            return First::match_one(current, ctx) || (Rest::match_one(current, ctx) || ...);
        }
    };
}
#endif //MREGEX_NODES_SET_HPP