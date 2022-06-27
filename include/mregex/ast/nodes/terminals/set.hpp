#ifndef MREGEX_NODES_SET_HPP
#define MREGEX_NODES_SET_HPP

#include <mregex/ast/nodes/terminals/terminal.hpp>
#include <mregex/ast/match_result.hpp>

namespace meta::ast
{
    template<typename First, typename... Rest>
    struct set : terminal
    {
        static_assert(is_trivially_matchable_v<First> && (is_trivially_matchable_v<Rest> && ...));

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            if (it == end)
                return {it, false};

            if (match_one(it, ctx))
                return cont(it + 1);
            return {it, false};
        }

        template<std::forward_iterator Iter, typename Context>
        static constexpr bool match_one(Iter current, Context &ctx) noexcept
        {
            return First::match_one(current, ctx) || (Rest::match_one(current, ctx) || ...);
        }
    };
}
#endif //MREGEX_NODES_SET_HPP