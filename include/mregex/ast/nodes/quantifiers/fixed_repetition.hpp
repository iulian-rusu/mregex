#ifndef MREGEX_NODES_FIXED_REPETITION_HPP
#define MREGEX_NODES_FIXED_REPETITION_HPP

#include <mregex/ast/astfwd.hpp>
#include <mregex/ast/match_result.hpp>
#include <mregex/ast/traits.hpp>
#include <mregex/utility/continuations.hpp>
#include <mregex/utility/distance.hpp>
#include <mregex/regex_context.hpp>

namespace meta::ast
{
    template<match_mode Mode, std::size_t N, typename Inner>
    struct basic_repetition<Mode, symbol::quantifier_value<N>, symbol::quantifier_value<N>, Inner>
    {
        static constexpr std::size_t capture_count = Inner::capture_count;

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            if constexpr (Mode == match_mode::possessive)
                return possessive_match(begin, end, it, ctx, cont);
            else
                return backtracking_match(begin, end, it, ctx, cont);
        }

    private:
        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto possessive_match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            if (auto result = backtracking_match(begin, end, it, ctx, continuations<Iter>::success))
                return cont(result.end);
            return {it, false};
        }

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto backtracking_match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires (!is_trivially_matchable<Inner>)
        {
            if constexpr (flags_of<Context>::unroll)
            {
                auto continuation = [=, &ctx, &cont](Iter new_it) noexcept {
                    return fixed_repetition<N - 1, Inner>::match(begin, end, new_it, ctx, cont);
                };
                return Inner::match(begin, end, it, ctx, continuation);
            }
            else
            {
                return non_unrolled_generic_match(begin, end, it, ctx, cont);
            }
        }

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto backtracking_match(Iter, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires is_trivially_matchable<Inner>
        {
            if (distance_less_than<N>(it, end))
                return {it, false};
            if constexpr (flags_of<Context>::unroll)
                return unrolled_trivial_match(it, ctx, cont, std::make_index_sequence<N>{});
            else
                return non_unrolled_trivial_match(it, ctx, cont);
        }

        template<std::random_access_iterator Iter, typename Context, typename Continuation, std::size_t... Indices>
        static constexpr auto unrolled_trivial_match(
                Iter it, Context &ctx, Continuation &&cont,
                std::index_sequence<Indices ...> &&
        ) noexcept -> match_result<Iter>
        {
            if ((Inner::match_one(std::next(it, Indices), ctx) && ...))
                return cont(std::next(it, N));
            return {it, false};
        }

        template<std::forward_iterator Iter, typename Context, typename Continuation, std::size_t... Indices>
        static constexpr auto unrolled_trivial_match(
                Iter it, Context &ctx, Continuation &&cont,
                std::index_sequence<Indices ...> &&
        ) noexcept -> match_result<Iter>
        {
            if (((Indices, Inner::match_one(it++, ctx)) && ...))
                return cont(it);
            return {it, false};
        }

        template<std::random_access_iterator Iter, typename Context, typename Continuation>
        static constexpr auto non_unrolled_trivial_match(Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            for (std::size_t offset = 0; offset != N; ++offset)
                if (!Inner::match_one(std::next(it, offset), ctx))
                    return {it, false};
            return cont(std::next(it, N));
        }

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto non_unrolled_trivial_match(Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            for (std::size_t match_count = 0; match_count != N; ++match_count)
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
}
#endif //MREGEX_NODES_FIXED_REPETITION_HPP