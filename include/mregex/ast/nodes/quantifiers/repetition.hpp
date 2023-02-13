#ifndef MREGEX_NODES_REPETITION_HPP
#define MREGEX_NODES_REPETITION_HPP

#include <mregex/ast/astfwd.hpp>
#include <mregex/ast/match_result.hpp>
#include <mregex/ast/traits.hpp>
#include <mregex/utility/continuations.hpp>

namespace meta::ast
{
    template<match_mode Mode, symbol::quantifier LowerBound, symbol::quantifier UpperBound, typename Inner>
    struct basic_repetition
    {
        static_assert(symbol::is_valid_range<LowerBound, UpperBound>, "invalid range bounds");

        using range_size = symbol::subtract_t<UpperBound, LowerBound>;

        static constexpr std::size_t capture_count = Inner::capture_count;

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            auto continuation = [=, &ctx, &cont](Iter new_it) noexcept {
                return match_between_bounds(begin, end, new_it, ctx, cont);
            };
            return basic_fixed_repetition<Mode, symbol::get_value<LowerBound>, Inner>::match(begin, end, it, ctx, continuation);
        }

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires symbol::is_zero<LowerBound>
        {
            return match_between_bounds(begin, end, it, ctx, cont);
        }

    private:
        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match_between_bounds(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            if constexpr (Mode == match_mode::possessive)
                return bounded_possessive_match<range_size>(begin, end, it, ctx, cont);
            else if constexpr ((Mode == match_mode::greedy) ^ Context::flags::ungreedy)
                return bounded_greedy_match<range_size>(begin, end, it, ctx, cont);
            else
                return bounded_lazy_match<range_size>(begin, end, it, ctx, cont);
        }

        template<symbol::quantifier Bound, std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto bounded_greedy_match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            if constexpr (!symbol::is_zero<Bound>)
            {
                auto continuation = [=, &ctx, &cont](Iter new_it) noexcept -> match_result<Iter> {
                    return bounded_greedy_match<symbol::decrement_t<Bound>>(begin, end, new_it, ctx, cont);
                };
                if (auto inner_match = Inner::match(begin, end, it, ctx, continue_unless_infinite_loop<Bound>(it, continuation)))
                    return inner_match;
            }
            return cont(it);
        }

        template<symbol::quantifier Bound, std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto bounded_greedy_match(Iter, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires (is_trivially_matchable<Inner> && std::bidirectional_iterator<Iter>)
        {
            Iter const start = it;
            for (std::size_t match_count = 0; !symbol::equals<Bound>(match_count); ++match_count)
            {
                if (it == end || !Inner::match_one(it, ctx))
                    break;
                ++it;
            }
            for (; it != start; --it)
            {
                if (auto rest_match = cont(it))
                    return rest_match;
            }
            return cont(start);
        }

        template<symbol::quantifier Bound, std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto bounded_lazy_match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            if (auto rest_match = cont(it))
                return rest_match;
            if constexpr (!symbol::is_zero<Bound>)
            {
                auto continuation = [=, &ctx, &cont](Iter new_it) noexcept -> match_result<Iter> {
                    return bounded_lazy_match<symbol::decrement_t<Bound>>(begin, end, new_it, ctx, cont);
                };
                return Inner::match(begin, end, it, ctx, continue_unless_infinite_loop<Bound>(it, continuation));
            }
            return {it, false};
        }

        template<symbol::quantifier Bound, std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto bounded_lazy_match(Iter, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires is_trivially_matchable<Inner>
        {
            for (std::size_t match_count = 0;; ++match_count, ++it)
            {
                if (auto rest_match = cont(it))
                    return rest_match;
                if (symbol::equals<Bound>(match_count) || it == end || !Inner::match_one(it, ctx))
                    break;
            }
            return {it, false};
        }

        template<symbol::quantifier Bound, std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto bounded_possessive_match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            for (std::size_t match_count = 0; !symbol::equals<Bound>(match_count); ++match_count)
            {
                auto inner_match = Inner::match(begin, end, it, ctx, continuations<Iter>::success);
                if (!inner_match)
                    break;
                it = inner_match.end;
            }
            return cont(it);
        }

        template<symbol::quantifier Bound, std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto bounded_possessive_match(Iter, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires is_trivially_matchable<Inner>
        {
            for (std::size_t match_count = 0; !symbol::equals<Bound>(match_count); ++match_count)
            {
                if (it == end || !Inner::match_one(it, ctx))
                    break;
                ++it;
            }
            return cont(it);
        }

    private:
        template<symbol::quantifier Bound, std::forward_iterator Iter, typename Continuation>
        static constexpr auto continue_unless_infinite_loop(Iter it, Continuation &&cont) noexcept
        {
            return [=, &cont](Iter new_it) noexcept -> match_result<Iter> {
                if (symbol::is_infinity<Bound> && is_zero_length_matcher<Inner> && new_it == it)
                    return {it, false};
                return cont(new_it);
            };
        }
    };

    template<match_mode Mode, typename Inner>
    struct basic_repetition<Mode, symbol::quantifier_value<1>, symbol::quantifier_value<1>, Inner> : Inner {};
}
#endif //MREGEX_NODES_REPETITION_HPP