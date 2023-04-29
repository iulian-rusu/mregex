#ifndef MREGEX_NODES_FIXED_REPETITION_HPP
#define MREGEX_NODES_FIXED_REPETITION_HPP

#include <mregex/ast/nodes/terminals/empty.hpp>
#include <mregex/ast/match_result.hpp>
#include <mregex/ast/traits.hpp>
#include <mregex/utility/continuations.hpp>
#include <mregex/utility/distance.hpp>

namespace meta::ast
{
    template<match_mode Mode, std::size_t N, typename Inner>
    struct basic_repetition<Mode, symbol::quantifier_value<N>, symbol::quantifier_value<N>, Inner>
    {
        static_assert(N > 1, "this specialization does not handle trivial repetitions");

        template<std::forward_iterator Iter, typename Context, match_continuation<Iter> Cont>
        static constexpr auto match(Iter begin, Iter end, Iter current, Context &ctx, Cont &&cont) noexcept
        -> match_result<Iter>
        {
            if constexpr (Mode == match_mode::possessive)
                return possessive_match(begin, end, current, ctx, cont);
            else
                return backtracking_match(begin, end, current, ctx, cont);
        }

    private:
        template<std::forward_iterator Iter, typename Context, match_continuation<Iter> Cont>
        static constexpr auto possessive_match(Iter begin, Iter end, Iter current, Context &ctx, Cont &&cont) noexcept
        -> match_result<Iter>
        {
            if (auto result = backtracking_match(begin, end, current, ctx, continuations<Iter>::success))
                return cont(result.end);
            return non_match(current);
        }

        template<std::forward_iterator Iter, typename Context, match_continuation<Iter> Cont>
        static constexpr auto backtracking_match(Iter begin, Iter end, Iter current, Context &ctx, Cont &&cont) noexcept
        -> match_result<Iter>
        {
            if constexpr (Context::flags::unroll)
                return unrolled_backtracking_match(begin, end, current, ctx, cont);
            else
                return non_unrolled_backtracking_match(begin, end, current, ctx, cont);
        }

        template<std::forward_iterator Iter, typename Context, match_continuation<Iter> Cont>
        static constexpr auto backtracking_match(Iter /*begin*/, Iter end, Iter current, Context &ctx, Cont &&cont) noexcept
        -> match_result<Iter>
        requires is_trivially_matchable<Inner>
        {
            if (distance_less_than<N>(current, end))
                return non_match(current);
            if constexpr (Context::flags::unroll)
                return unrolled_trivial_match(current, ctx, cont, std::make_index_sequence<N>{});
            else
                return non_unrolled_trivial_match(current, ctx, cont);
        }

        template<std::forward_iterator Iter, typename Context, match_continuation<Iter> Cont>
        static constexpr auto unrolled_backtracking_match(Iter begin, Iter end, Iter current, Context &ctx, Cont &&cont) noexcept
        -> match_result<Iter>
        {
            auto continuation = [=, &ctx, &cont](Iter next) noexcept -> match_result<Iter> {
                return fixed_repetition<N - 1, Inner>::match(begin, end, next, ctx, cont);
            };
            return Inner::match(begin, end, current, ctx, continuation);
        }

        template<std::forward_iterator Iter, typename Context, match_continuation<Iter> Cont>
        static constexpr auto non_unrolled_backtracking_match(
                Iter begin, Iter end, Iter current,
                Context &ctx, Cont &&cont,
                std::size_t repeats = N
        ) noexcept -> match_result<Iter>
        {
            if (repeats == 1)
                return Inner::match(begin, end, current, ctx, cont);
            auto continuation = [=, &ctx, &cont](Iter next) noexcept -> match_result<Iter> {
                return non_unrolled_backtracking_match(begin, end, next, ctx, cont, repeats - 1);
            };
            return Inner::match(begin, end, current, ctx, continuation);
        }

        template<std::random_access_iterator Iter, typename Context, match_continuation<Iter> Cont, std::size_t... Indices>
        static constexpr auto unrolled_trivial_match(
                Iter current, Context &ctx, Cont &&cont,
                std::index_sequence<Indices ...>
        ) noexcept -> match_result<Iter>
        {
            if ((Inner::match_one(std::next(current, Indices), ctx) && ...))
                return cont(std::next(current, N));
            return non_match(current);
        }

        template<std::forward_iterator Iter, typename Context, match_continuation<Iter> Cont, std::size_t... Indices>
        static constexpr auto unrolled_trivial_match(
                Iter current, Context &ctx, Cont &&cont,
                std::index_sequence<Indices ...>
        ) noexcept -> match_result<Iter>
        {
            if (((Indices, Inner::match_one(current++, ctx)) && ...))
                return cont(current);
            return non_match(current);
        }

        template<std::random_access_iterator Iter, typename Context, match_continuation<Iter> Cont>
        static constexpr auto non_unrolled_trivial_match(Iter current, Context &ctx, Cont &&cont) noexcept
        -> match_result<Iter>
        {
            for (std::size_t offset = 0; offset != N; ++offset)
            {
                if (!Inner::match_one(std::next(current, offset), ctx))
                    return non_match(current);
            }
            return cont(std::next(current, N));
        }

        template<std::forward_iterator Iter, typename Context, match_continuation<Iter> Cont>
        static constexpr auto non_unrolled_trivial_match(Iter current, Context &ctx, Cont &&cont) noexcept
        -> match_result<Iter>
        {
            for (std::size_t match_count = 0; match_count != N; ++match_count)
            {
                if (!Inner::match_one(current++, ctx))
                    return non_match(current);
            }
            return cont(current);
        }
    };

    /**
     * @note A possessive quantifier with exactly 1 repetition is essentially an atomic group.
     * This specialization is needed to implement atomic matching for non-trivial AST nodes.
     * For trivial nodes, atomic groups have no effect and can be ignored.
     */
    template<typename Inner>
    requires (!is_trivially_matchable<Inner>)
    struct basic_repetition<match_mode::possessive, symbol::quantifier_value<1>, symbol::quantifier_value<1>, Inner>
    {
        template<std::forward_iterator Iter, typename Context, match_continuation<Iter> Cont>
        static constexpr auto match(Iter begin, Iter end, Iter current, Context &ctx, Cont &&cont) noexcept
        -> match_result<Iter>
        {
            if (auto inner_match = Inner::match(begin, end, current, ctx, continuations<Iter>::success))
                return cont(inner_match.end);
            return non_match(current);
        }
    };

    template<match_mode Mode, typename Inner>
    struct basic_repetition<Mode, symbol::quantifier_value<1>, symbol::quantifier_value<1>, Inner> : Inner {};

    template<match_mode Mode, typename Inner>
    struct basic_repetition<Mode, symbol::quantifier_value<0>, symbol::quantifier_value<0>, Inner> : empty {};
}
#endif //MREGEX_NODES_FIXED_REPETITION_HPP