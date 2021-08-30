#ifndef META_STAR_HPP
#define META_STAR_HPP

#include "astfwd.hpp"
#include "ast_traits.hpp"
#include "match_result.hpp"

namespace meta::ast
{
    namespace impl
    {
        /**
         * Namespace with different strategies for implementing star cycles.
         * Currently greedy and lazy matching is supported.
         */

        template<typename Inner>
        struct greedy_match_strategy
        {
            template<std::forward_iterator Iter, typename Context, typename Continuation>
            static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
            -> match_result<Iter>
            requires (!is_trivially_matchable_v<Inner> || !std::bidirectional_iterator<Iter>)
            {
                auto continuation = [=, &ctx, &cont](Iter new_it) -> match_result<Iter> {
                    if (new_it == it)
                        return {new_it, false};
                    return greedy_match_strategy<Inner>::match(begin, end, new_it, ctx, cont);
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
                std::forward_iterator auto start = it;
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
        struct lazy_match_strategy
        {
            template<std::forward_iterator Iter, typename Context, typename Continuation>
            static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
            -> match_result<Iter>
            requires (!is_trivially_matchable_v<Inner>)
            {
                auto continuation = [=, &ctx, &cont](Iter new_it) -> match_result<Iter> {
                    if (new_it == it)
                        return {new_it, false};
                    return lazy_match_strategy<Inner>::match(begin, end, new_it, ctx, cont);
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

    template<typename Inner, bool lazy>
    using match_strategy =
            std::conditional_t
            <
                lazy,
                impl::lazy_match_strategy<Inner>,
                impl::greedy_match_strategy<Inner>
            >;

    template<typename Inner>
    struct star
    {
        static constexpr std::size_t capture_count = Inner::capture_count;

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            using strategy = match_strategy<Inner, flags<Context>::ungreedy>;

            return strategy::match(begin, end, it, ctx, cont);
        }
    };
}
#endif //META_STAR_HPP
