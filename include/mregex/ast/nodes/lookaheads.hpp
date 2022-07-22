#ifndef MREGEX_NODES_LOOKAHEADS_HPP
#define MREGEX_NODES_LOOKAHEADS_HPP

#include <mregex/ast/astfwd.hpp>
#include <mregex/ast/traits.hpp>
#include <mregex/utility/continuations.hpp>

namespace meta::ast
{
    template<typename Inner>
    struct lookahead_matcher
    {
        template<std::bidirectional_iterator Iter, typename Context>
        static constexpr bool match(Iter begin, Iter end, Iter it, Context &ctx) noexcept
        requires (!is_trivially_matchable_v<Inner>)
        {
            auto result = Inner::match(begin, end, it, ctx, continuations<Iter>::success);
            return result.matched;
        }

        template<std::bidirectional_iterator Iter, typename Context>
        static constexpr bool match(Iter, Iter end, Iter it, Context &ctx) noexcept
        requires is_trivially_matchable_v<Inner>
        {
            return it != end && Inner::match_one(it, ctx);
        }
    };

    template<typename Inner>
    struct positive_lookahead
    {
        static constexpr std::size_t capture_count = Inner::capture_count;

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            if (lookahead_matcher<Inner>::match(begin, end, it, ctx))
                return cont(it);
            return {it, false};
        }
    };

    template<typename Inner>
    struct negative_lookahead
    {
        static constexpr std::size_t capture_count = Inner::capture_count;

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            if (lookahead_matcher<Inner>::match(begin, end, it, ctx))
                return {it, false};
            return cont(it);
        }
    };
}
#endif //MREGEX_NODES_LOOKAHEADS_HPP