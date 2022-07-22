#ifndef MREGEX_NODES_SEQUENCE_HPP
#define MREGEX_NODES_SEQUENCE_HPP

#include <mregex/ast/astfwd.hpp>
#include <mregex/ast/match_result.hpp>
#include <mregex/ast/traits.hpp>
#include <mregex/utility/distance.hpp>
#include <mregex/regex_context.hpp>

namespace meta::ast
{
    template<typename First, typename... Rest>
    struct sequence
    {
        static constexpr std::size_t capture_count = capture_count_v<First, Rest ...>;

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            auto continuation = [=, &ctx, &cont](Iter new_it) noexcept {
                return sequence<Rest ...>::match(begin, end, new_it, ctx, cont);
            };
            return First::match(begin, end, it, ctx, continuation);
        }

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires is_trivially_matchable_v<First>
        {
            if (it == end)
                return {it, false};
            if (First::match_one(it, ctx))
                return sequence<Rest ...>::match(begin, end, std::next(it), ctx, cont);
            return {it, false};
        }
    };

    template<typename First>
    struct sequence<First> : First {};
}
#endif //MREGEX_NODES_SEQUENCE_HPP