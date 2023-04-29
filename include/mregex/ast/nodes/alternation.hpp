#ifndef MREGEX_NODES_ALTERNATION_HPP
#define MREGEX_NODES_ALTERNATION_HPP

#include <mregex/ast/nodes/set.hpp>
#include <mregex/ast/match_result.hpp>
#include <mregex/ast/traits.hpp>
#include <mregex/utility/continuations.hpp>

namespace meta::ast
{
    template<typename First, typename... Rest>
    struct alternation
    {
        template<std::forward_iterator Iter, typename Context, match_continuation<Iter> Cont>
        static constexpr auto match(Iter begin, Iter end, Iter current, Context &ctx, Cont &&cont) noexcept
        -> match_result<Iter>
        {
            if (auto first_match = First::match(begin, end, current, ctx, cont))
                return first_match;
            return alternation<Rest ...>::match(begin, end, current, ctx, cont);
        }
    };

    template<typename First, typename... Rest>
    requires are_trivially_matchable<First, Rest ...>
    struct alternation<First, Rest ...> : set<First, Rest ...> {};

    template<typename Inner>
    struct alternation<Inner> : Inner {};
}
#endif //MREGEX_NODES_ALTERNATION_HPP