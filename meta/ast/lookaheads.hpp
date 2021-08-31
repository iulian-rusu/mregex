#ifndef META_LOOKAHEADS_HPP
#define META_LOOKAHEADS_HPP

#include "astfwd.hpp"
#include "ast_traits.hpp"
#include "../utility/continuations.hpp"

namespace meta::ast
{
    template<typename Inner>
    struct positive_lookahead
    {
        static constexpr std::size_t capture_count = Inner::capture_count;

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires (!is_trivially_matchable_v<Inner>)
        {
            if (Inner::match(begin, end, it, ctx, continuations<Iter>::epsilon))
                return cont(it);
            return {it, false};
        }

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires is_trivially_matchable_v<Inner>
        {
            if (it != end && Inner::consume_one(it, ctx))
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
        requires (!is_trivially_matchable_v<Inner>)
        {
            if (Inner::match(begin, end, it, ctx, continuations<Iter>::epsilon))
                return {it, false};
            return cont(it);
        }

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires is_trivially_matchable_v<Inner>
        {
            if (it != end && Inner::consume_one(it, ctx))
                return {it, false};
            return cont(it);
        }
    };
}
#endif //META_LOOKAHEADS_HPP