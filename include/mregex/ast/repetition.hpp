#ifndef MREGEX_REPETITION_HPP
#define MREGEX_REPETITION_HPP

#include <mregex/ast/astfwd.hpp>
#include <mregex/ast/ast_traits.hpp>
#include <mregex/ast/match_result.hpp>

namespace meta::ast
{
    template<std::size_t A, typename Inner>
    using exact_repetition = repetition<symbol::quantifier_value<A>, symbol::quantifier_value<A>, Inner>;

    template<std::size_t A, std::size_t B, typename Inner>
    struct repetition<symbol::quantifier_value<A>, symbol::quantifier_value<B>, Inner>
    {
        static_assert(A < B, "invalid repetition bounds");

        static constexpr std::size_t R = B - A;
        static constexpr std::size_t capture_count = Inner::capture_count;

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires (A > 0u)
        {
            auto continuation = [=, &ctx, &cont](Iter new_it) noexcept {
                return consume_rest(begin, end, new_it, ctx, cont);
            };
            return exact_repetition<A, Inner>::match(begin, end, it, ctx, continuation);
        }

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires (A == 0u)
        {
            return consume_rest(begin, end, it, ctx, cont);
        }

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto consume_rest(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires (!is_trivially_matchable_v<Inner>)
        {
            using next_repetition = repetition<symbol::quantifier_value<0>, symbol::quantifier_value<R - 1>, Inner>;
            auto continuation = [=, &ctx, &cont](Iter new_it) noexcept {
                return next_repetition::match(begin, end, new_it, ctx, cont);
            };
            if (auto inner_match = Inner::match(begin, end, it, ctx, continuation))
                return inner_match;
            return cont(it);
        }

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto consume_rest(Iter, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires is_trivially_matchable_v<Inner>
        {
            std::size_t matched_so_far = 0;
            for (; it != end && matched_so_far < R; ++it)
            {
                if (!Inner::consume_one(it, ctx))
                    break;
                ++matched_so_far;
            }
            return cont(it);
        }
    };

    template<std::size_t N, typename Inner>
    struct repetition<symbol::quantifier_value<N>, symbol::quantifier_inf, Inner>
    {
        static constexpr std::size_t capture_count = Inner::capture_count;

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            auto continuation = [=, &ctx, &cont](Iter new_it) noexcept {
                return star<Inner>::match(begin, end, new_it, ctx, cont);
            };
            return exact_repetition<N, Inner>::match(begin, end, it, ctx, continuation);
        }
    };

    template<std::size_t N, typename Inner>
    struct repetition<symbol::quantifier_value<N>, symbol::quantifier_value<N>, Inner>
    {
        static constexpr std::size_t capture_count = Inner::capture_count;

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires (!is_trivially_matchable_v<Inner>)
        {
            auto continuation = [=, &ctx, &cont](Iter new_it) noexcept {
                return exact_repetition<N - 1, Inner>::match(begin, end, new_it, ctx, cont);
            };
            return Inner::match(begin, end, it, ctx, continuation);
        }

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires is_trivially_matchable_v<Inner>
        {
            std::size_t const remaining_length = std::distance(it, end);
            if (remaining_length < N)
                return {it, false};

            std::size_t matched = 0;
            for (; matched < N; ++matched)
            {
                if (!Inner::consume_one(it++, ctx))
                    return {it, false};
            }
            return cont(it);
        }
    };

    template<typename Inner>
    struct repetition<symbol::quantifier_value<0u>, symbol::quantifier_value<0u>, Inner>
    {
        static constexpr std::size_t capture_count = Inner::capture_count;

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter, Iter, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            return cont(it);
        }
    };

    template<typename Inner>
    struct repetition<symbol::quantifier_value<0u>, symbol::quantifier_inf, Inner> : star<Inner> {};
}
#endif //MREGEX_REPETITION_HPP