#ifndef MREGEX_NODES_REPETITION_HPP
#define MREGEX_NODES_REPETITION_HPP

#include <mregex/ast/astfwd.hpp>
#include <mregex/ast/match_result.hpp>
#include <mregex/ast/traits.hpp>
#include <mregex/utility/continuations.hpp>

namespace meta::ast
{
    template<match_mode Mode, symbol::finite_quantifier LowerBound, symbol::quantifier UpperBound, typename Inner>
    struct basic_repetition
    {
        static_assert(symbol::is_valid_range<LowerBound, UpperBound>, "invalid range bounds");

        using range_size = symbol::subtract_t<UpperBound, LowerBound>;

        static constexpr std::size_t capture_count = Inner::capture_count;

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter current, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            auto continuation = [=, &ctx, &cont](Iter next) noexcept {
                return match_between_bounds(begin, end, next, ctx, cont);
            };
            return basic_fixed_repetition<Mode, symbol::get_value<LowerBound>, Inner>::match(begin, end, current, ctx, continuation);
        }

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter current, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires symbol::is_zero<LowerBound>
        {
            return match_between_bounds(begin, end, current, ctx, cont);
        }

    private:
        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match_between_bounds(Iter begin, Iter end, Iter current, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            if constexpr (Mode == match_mode::possessive)
                return bounded_possessive_match<range_size>(begin, end, current, ctx, cont);
            else if constexpr ((Mode == match_mode::greedy) ^ Context::flags::ungreedy)
                return bounded_greedy_match<range_size>(begin, end, current, ctx, cont);
            else
                return bounded_lazy_match<range_size>(begin, end, current, ctx, cont);
        }

        template<symbol::quantifier Bound, std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto bounded_greedy_match(Iter begin, Iter end, Iter current, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            if constexpr (!symbol::is_zero<Bound>)
            {
                auto continuation = [=, &ctx, &cont](Iter next) noexcept -> match_result<Iter> {
                    return bounded_greedy_match<symbol::decrement_t<Bound>>(begin, end, next, ctx, cont);
                };
                if (auto inner_match = Inner::match(begin, end, current, ctx, continue_unless_infinite_loop<Bound>(current, continuation)))
                    return inner_match;
            }
            return cont(current);
        }

        template<symbol::quantifier Bound, std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto bounded_greedy_match(Iter /*begin*/, Iter end, Iter current, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires (is_trivially_matchable<Inner> && std::bidirectional_iterator<Iter>)
        {
            Iter const initial = current;
            for (std::size_t match_count = 0; !symbol::equals<Bound>(match_count); ++match_count)
            {
                if (current == end || !Inner::match_one(current, ctx))
                    break;
                ++current;
            }
            for (; current != initial; --current)
            {
                if (auto rest_match = cont(current))
                    return rest_match;
            }
            return cont(initial);
        }

        template<symbol::quantifier Bound, std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto bounded_lazy_match(Iter begin, Iter end, Iter current, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            if (auto rest_match = cont(current))
                return rest_match;
            if constexpr (!symbol::is_zero<Bound>)
            {
                auto continuation = [=, &ctx, &cont](Iter next) noexcept -> match_result<Iter> {
                    return bounded_lazy_match<symbol::decrement_t<Bound>>(begin, end, next, ctx, cont);
                };
                return Inner::match(begin, end, current, ctx, continue_unless_infinite_loop<Bound>(current, continuation));
            }
            return {current, false};
        }

        template<symbol::quantifier Bound, std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto bounded_lazy_match(Iter /*begin*/, Iter end, Iter current, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires is_trivially_matchable<Inner>
        {
            for (std::size_t match_count = 0;; ++match_count, ++current)
            {
                if (auto rest_match = cont(current))
                    return rest_match;
                if (symbol::equals<Bound>(match_count) || current == end || !Inner::match_one(current, ctx))
                    break;
            }
            return {current, false};
        }

        template<symbol::quantifier Bound, std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto bounded_possessive_match(Iter begin, Iter end, Iter current, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            for (std::size_t match_count = 0; !symbol::equals<Bound>(match_count); ++match_count)
            {
                auto inner_match = Inner::match(begin, end, current, ctx, continuations<Iter>::success);
                if (!inner_match)
                    break;
                current = inner_match.end;
            }
            return cont(current);
        }

        template<symbol::quantifier Bound, std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto bounded_possessive_match(Iter /*begin*/, Iter end, Iter current, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires is_trivially_matchable<Inner>
        {
            for (std::size_t match_count = 0; !symbol::equals<Bound>(match_count); ++match_count)
            {
                if (current == end || !Inner::match_one(current, ctx))
                    break;
                ++current;
            }
            return cont(current);
        }
        
        template<symbol::quantifier Bound, std::forward_iterator Iter, typename Continuation>
        static constexpr auto continue_unless_infinite_loop(Iter current, Continuation &&cont) noexcept
        {
            return [=, &cont](Iter next) noexcept -> match_result<Iter> {
                if (symbol::is_infinity<Bound> && is_zero_length_matcher<Inner> && current == next)
                    return {current, false};
                return cont(next);
            };
        }
    };

    template<match_mode Mode, typename Inner>
    struct basic_repetition<Mode, symbol::quantifier_value<1>, symbol::quantifier_value<1>, Inner> : Inner {};
}
#endif //MREGEX_NODES_REPETITION_HPP