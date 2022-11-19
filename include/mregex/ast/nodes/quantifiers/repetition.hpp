#ifndef MREGEX_NODES_REPETITION_HPP
#define MREGEX_NODES_REPETITION_HPP

#include <mregex/ast/astfwd.hpp>
#include <mregex/ast/match_result.hpp>
#include <mregex/ast/traits.hpp>
#include <mregex/utility/continuations.hpp>
#include <mregex/utility/distance.hpp>
#include <mregex/regex_context.hpp>

namespace meta::ast
{
    template<match_mode Mode, std::size_t A, std::size_t B, typename Inner>
    struct basic_repetition<Mode, symbol::quantifier_value<A>, symbol::quantifier_value<B>, Inner>
    {
        static_assert(A < B, "invalid repetition bounds");

        static constexpr std::size_t rest_size = B - A;
        static constexpr std::size_t capture_count = Inner::capture_count;

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires (A > 0)
        {
            auto continuation = [=, &ctx, &cont](Iter new_it) noexcept {
                return match_rest(begin, end, new_it, ctx, cont);
            };
            return basic_fixed_repetition<Mode, A, Inner>::match(begin, end, it, ctx, continuation);
        }

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires (A == 0)
        {
            return match_rest(begin, end, it, ctx, cont);
        }

    private:
        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match_rest(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            if constexpr (Mode == match_mode::possessive)
                return possessive_match_rest<rest_size>(begin, end, it, ctx, cont);
            else if constexpr ((Mode == match_mode::greedy) ^ context_flags<Context>::ungreedy)
                return greedy_match_rest<rest_size>(begin, end, it, ctx, cont);
            else
                return lazy_match_rest<rest_size>(begin, end, it, ctx, cont);
        }

        template<std::size_t N, std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto greedy_match_rest(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires (!is_trivially_matchable<Inner> || !std::bidirectional_iterator<Iter>)
        {
            if constexpr (N > 0)
            {
                auto continuation = [=, &ctx, &cont](Iter new_it) noexcept {
                    return greedy_match_rest<N - 1>(begin, end, new_it, ctx, cont);
                };
                if (auto inner_match = Inner::match(begin, end, it, ctx, continuation))
                    return inner_match;
            }
            return cont(it);
        }

        template<std::size_t N, std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto greedy_match_rest(Iter, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires (is_trivially_matchable<Inner> && std::bidirectional_iterator<Iter>)
        {
            Iter const start = it;
            for (std::size_t match_count = 0; match_count != N && it != end; ++match_count, ++it)
                if (!Inner::match_one(it, ctx))
                    break;
            for (; it != start; --it)
                if (auto rest_match = cont(it))
                    return rest_match;
            return cont(start);
        }

        template<std::size_t N, std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto lazy_match_rest(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires (!is_trivially_matchable<Inner>)
        {
            if (auto rest_match = cont(it))
                return rest_match;
            if constexpr (N > 0)
            {
                auto continuation = [=, &ctx, &cont](Iter new_it) noexcept {
                    return lazy_match_rest<N - 1>(begin, end, new_it, ctx, cont);
                };
                return Inner::match(begin, end, it, ctx, continuation);
            }
            else
            {
                return {it, false};
            }
        }

        template<std::size_t N, std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto lazy_match_rest(Iter, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires is_trivially_matchable<Inner>
        {
            for (std::size_t match_count = 0;; ++match_count, ++it)
            {
                if (auto rest_match = cont(it))
                    return rest_match;
                if (match_count == N || it == end)
                    break;
                if (!Inner::match_one(it, ctx))
                    break;
            }
            return {it, false};
        }

        template<std::size_t N, std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto possessive_match_rest(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires (!is_trivially_matchable<Inner>)
        {
            for (std::size_t match_count = 0; match_count != N; ++match_count)
                if (auto inner_match = Inner::match(begin, end, it, ctx, continuations<Iter>::success))
                    it = inner_match.end;
                else
                    break;
            return cont(it);
        }

        template<std::size_t N, std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto possessive_match_rest(Iter, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires is_trivially_matchable<Inner>
        {
            for (std::size_t match_count = 0; match_count != N && it != end; ++match_count, ++it)
                if (!Inner::match_one(it, ctx))
                    break;
            return cont(it);
        }
    };

    template<match_mode Mode, std::size_t N, typename Inner>
    struct basic_repetition<Mode, symbol::quantifier_value<N>, symbol::quantifier_inf, Inner>
    {
        static constexpr std::size_t capture_count = Inner::capture_count;

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            auto continuation = [=, &ctx, &cont](Iter new_it) noexcept {
                return basic_star<Mode, Inner>::match(begin, end, new_it, ctx, cont);
            };
            return basic_fixed_repetition<Mode, N, Inner>::match(begin, end, it, ctx, continuation);
        }
    };

    template<match_mode Mode, typename Inner>
    struct basic_repetition<Mode, symbol::quantifier_value<1>, symbol::quantifier_value<1>, Inner> : Inner {};
}
#endif //MREGEX_NODES_REPETITION_HPP