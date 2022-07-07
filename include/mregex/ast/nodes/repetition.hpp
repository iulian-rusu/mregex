#ifndef MREGEX_NODES_REPETITION_HPP
#define MREGEX_NODES_REPETITION_HPP

#include <mregex/ast/astfwd.hpp>
#include <mregex/ast/ast_traits.hpp>
#include <mregex/ast/match_result.hpp>
#include <mregex/utility/distance.hpp>
#include <mregex/regex_context.hpp>

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
        requires (A > 0)
        {
            auto continuation = [=, &ctx, &cont](Iter new_it) noexcept {
                return match_rest(begin, end, new_it, ctx, cont);
            };
            return exact_repetition<A, Inner>::match(begin, end, it, ctx, continuation);
        }

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires (A == 0)
        {
            return match_rest(begin, end, it, ctx, cont);
        }

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match_rest(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
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
        static constexpr auto match_rest(Iter, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires is_trivially_matchable_v<Inner>
        {
            for (std::size_t matched = 0; it != end && matched < R; ++matched, ++it)
                if (!Inner::match_one(it, ctx))
                    break;
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
            if constexpr (flags_of<Context>::unroll)
            {
                auto continuation = [=, &ctx, &cont](Iter new_it) noexcept {
                    return exact_repetition<N - 1, Inner>::match(begin, end, new_it, ctx, cont);
                };
                return Inner::match(begin, end, it, ctx, continuation);
            }
            else
            {
                return non_unrolled_generic_match(begin, end, it, ctx, cont);
            }
        }

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires is_trivially_matchable_v<Inner>
        {
            if (distance_less_than<N>(it, end))
                return {it, false};
            if constexpr (flags_of<Context>::unroll)
                return unrolled_trivial_match(it, ctx, cont, std::make_index_sequence<N>{});
            else
                return non_unrolled_trivial_match(it, ctx, cont);
        }

    private:
        template<std::forward_iterator Iter, typename Context, typename Continuation, std::size_t... Indices>
        static constexpr auto unrolled_trivial_match(
                Iter it, Context &ctx, Continuation &&cont,
                std::index_sequence<Indices ...> &&
        ) noexcept -> match_result<Iter>
        {
            if ((Inner::match_one(it + Indices, ctx) && ...))
                return cont(it + N);
            return {it, false};
        }

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto non_unrolled_trivial_match(Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires std::random_access_iterator<Iter>
        {
            for (std::size_t offset = 0; offset < N; ++offset)
                if (!Inner::match_one(it + offset, ctx))
                    return {it, false};
            return cont(it + N);
        }

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto non_unrolled_trivial_match(Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires (!std::random_access_iterator<Iter>)
        {
            for (std::size_t matched = 0; matched < N; ++matched)
                if (!Inner::match_one(it++, ctx))
                    return {it, false};
            return cont(it);
        }

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto non_unrolled_generic_match(
                Iter begin, Iter end, Iter it,
                Context &ctx, Continuation &&cont,
                std::size_t repeats = N
        ) noexcept -> match_result<Iter>
        {
            if (repeats == 1)
                return Inner::match(begin, end, it, ctx, cont);
            auto continuation = [=, &ctx, &cont](Iter new_it) noexcept {
                return non_unrolled_generic_match(begin, end, new_it, ctx, cont, repeats - 1);
            };
            return Inner::match(begin, end, it, ctx, continuation);
        }
    };

    template<typename Inner>
    struct repetition<symbol::quantifier_value<0>, symbol::quantifier_value<0>, Inner>
    {
        static constexpr std::size_t capture_count = Inner::capture_count;

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter, Iter, Iter it, Context &, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            return cont(it);
        }
    };

    template<typename Inner>
    struct repetition<symbol::quantifier_value<1>, symbol::quantifier_value<1>, Inner> : Inner {};

    template<typename Inner>
    struct repetition<symbol::quantifier_value<0>, symbol::quantifier_inf, Inner> : star<Inner> {};
}
#endif //MREGEX_NODES_REPETITION_HPP