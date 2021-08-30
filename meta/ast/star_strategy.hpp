#ifndef META_STAR_STRATEGY_HPP
#define META_STAR_STRATEGY_HPP

#include "match_result.hpp"

namespace meta::ast
{
    /**
     * Helper struct to dispatch between lazy and greedy strategies for matching star cycles.
     *
     * @tparam Inner    The inner node to be matched in a star cycle
     */
    template<typename Inner, bool = false>
    struct star_strategy
    {
        // Greedy star algorithm

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires (!is_trivially_matchable_v<Inner> || !std::bidirectional_iterator<Iter>)
        {
            auto continuation = [=, &ctx, &cont](Iter new_it) -> match_result<Iter> {
                if (new_it == it)
                    return {new_it, false};
                return star_strategy<Inner, false>::match(begin, end, new_it, ctx, cont);
            };
            if (auto inner_match = Inner::match(begin, end, it, ctx, continuation))
                return inner_match;
            return cont(it);
        }

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires (is_trivially_matchable_v<Inner> && std::bidirectional_iterator<Iter>)
        {
            auto start = it;
            for (; it != end; ++it)
            {
                if (!Inner::consume_one(it, ctx))
                    break;
            }
            for (; it != start; --it)
            {
                if (auto rest_match = cont(it))
                    return rest_match;
            }
            return cont(it);
        }
    };

    template<typename Inner>
    struct star_strategy<Inner, true>
    {
        // Lazy star algorithm

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires (!is_trivially_matchable_v<Inner>)
        {
            auto continuation = [=, &ctx, &cont](Iter new_it) -> match_result<Iter> {
                if (new_it == it)
                    return {new_it, false};
                return star_strategy<Inner, true>::match(begin, end, new_it, ctx, cont);
            };
            if (auto rest_match = cont(it))
                return rest_match;
            return Inner::match(begin, end, it, ctx, continuation);
        }

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires is_trivially_matchable_v<Inner>
        {
            for (; it != end; ++it)
            {
                if (auto rest_match = cont(it))
                    return rest_match;
                if (!Inner::consume_one(it, ctx))
                    break;
            }
            return cont(it);
        }
    };
}
#endif //META_STAR_STRATEGY_HPP
