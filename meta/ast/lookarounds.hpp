#ifndef META_LOOKAROUNDS_HPP
#define META_LOOKAROUNDS_HPP

#include "astfwd.hpp"
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
        {
            if (Inner::match(begin, end, it, ctx, continuations<Iter>::epsilon))
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
            if (!Inner::match(begin, end, it, ctx, continuations<Iter>::epsilon))
                return cont(it);
            return {it, false};
        }
    };
}
#endif //META_LOOKAROUNDS_HPP