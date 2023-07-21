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
        static_assert(!std::is_same_v<LowerBound, UpperBound>, "this implementation does not handle fixed repetitions");

        template<std::forward_iterator Iter, typename Context, match_continuation<Iter> Cont>
        static constexpr auto match(Iter begin, Iter end, Iter current, Context &ctx, Cont &&cont) noexcept
        -> match_result<Iter>
        {
            auto continuation = [=, &ctx, &cont](Iter next) noexcept -> match_result<Iter> {
                return match_between_bounds(begin, end, next, ctx, cont);
            };
            return basic_fixed_repetition<Mode, symbol::get_value<LowerBound>, Inner>::match(begin, end, current, ctx, continuation);
        }

        template<std::forward_iterator Iter, typename Context, match_continuation<Iter> Cont>
        static constexpr auto match(Iter begin, Iter end, Iter current, Context &ctx, Cont &&cont) noexcept
        -> match_result<Iter>
        requires symbol::is_zero<LowerBound>
        {
            return match_between_bounds(begin, end, current, ctx, cont);
        }

    private:
        template<std::forward_iterator Iter, typename Context, match_continuation<Iter> Cont>
        static constexpr auto match_between_bounds(Iter begin, Iter end, Iter current, Context &ctx, Cont &&cont) noexcept
        -> match_result<Iter>
        {
            using range_size = symbol::subtract_t<UpperBound, LowerBound>;

            if constexpr (Mode == match_mode::possessive)
                return bounded_possessive_match<range_size>(begin, end, current, ctx, cont);
            else if constexpr ((Mode == match_mode::greedy) ^ Context::flags::ungreedy)
                return bounded_greedy_match<range_size>(begin, end, current, ctx, cont);
            else
                return bounded_lazy_match<range_size>(begin, end, current, ctx, cont);
        }

        template<symbol::quantifier Bound, std::forward_iterator Iter, typename Context, match_continuation<Iter> Cont>
        static constexpr auto bounded_possessive_match(Iter begin, Iter end, Iter current, Context &ctx, Cont &&cont) noexcept
        -> match_result<Iter>
        {
            for (std::size_t match_count = 0; !symbol::equals<Bound>(match_count); ++match_count)
            {
                auto inner_match = Inner::match(begin, end, current, ctx, continuations<Iter>::success);
                if (!inner_match || will_loop_forever(current, inner_match.end))
                    break;
                current = inner_match.end;
            }
            return cont(current);
        }

        template<symbol::quantifier Bound, std::forward_iterator Iter, typename Context, match_continuation<Iter> Cont>
        static constexpr auto bounded_possessive_match(Iter /*begin*/, Iter end, Iter current, Context &ctx, Cont &&cont) noexcept
        -> match_result<Iter>
        requires is_trivially_matchable<Inner>
        {
            for (std::size_t match_count = 0; !symbol::equals<Bound>(match_count); ++match_count)
            {
                if (current == end || !Inner::match_one(*current, ctx))
                    break;
                ++current;
            }
            return cont(current);
        }

        template<symbol::quantifier Bound, std::forward_iterator Iter, typename Context, match_continuation<Iter> Cont>
        static constexpr auto bounded_greedy_match(Iter begin, Iter end, Iter current, Context &ctx, Cont &&cont) noexcept
        -> match_result<Iter>
        {
            if constexpr (!symbol::is_zero<Bound>)
            {
                auto continuation = [=, &ctx, &cont](Iter next) noexcept -> match_result<Iter> {
                    return bounded_greedy_match<symbol::decrement_t<Bound>>(begin, end, next, ctx, cont);
                };
                if (auto inner_match = Inner::match(begin, end, current, ctx, continue_unless_infinite_loop(current, continuation)))
                    return inner_match;
            }
            return cont(current);
        }

        template<symbol::quantifier Bound, std::bidirectional_iterator Iter, typename Context, match_continuation<Iter> Cont>
        static constexpr auto bounded_greedy_match(Iter begin, Iter end, Iter current, Context &ctx, Cont &&cont) noexcept
        -> match_result<Iter>
        requires is_trivially_matchable<Inner>
        {
            auto continuation = [initial = current, &cont](Iter next) noexcept -> match_result<Iter> {
                while (true)
                {
                    if (auto rest_match = cont(next))
                        return rest_match;
                    if (next == initial)
                        break;
                    --next;
                }
                return non_match(initial);
            };
            return bounded_possessive_match<Bound>(begin, end, current, ctx, continuation);
        }

        template<symbol::quantifier Bound, std::forward_iterator Iter, typename Context, match_continuation<Iter> Cont>
        static constexpr auto bounded_lazy_match(Iter begin, Iter end, Iter current, Context &ctx, Cont &&cont) noexcept
        -> match_result<Iter>
        {
            if (auto rest_match = cont(current))
                return rest_match;
            if constexpr (!symbol::is_zero<Bound>)
            {
                auto continuation = [=, &ctx, &cont](Iter next) noexcept -> match_result<Iter> {
                    return bounded_lazy_match<symbol::decrement_t<Bound>>(begin, end, next, ctx, cont);
                };
                return Inner::match(begin, end, current, ctx, continue_unless_infinite_loop(current, continuation));
            }
            return non_match(current);
        }

        template<symbol::quantifier Bound, std::forward_iterator Iter, typename Context, match_continuation<Iter> Cont>
        static constexpr auto bounded_lazy_match(Iter /*begin*/, Iter end, Iter current, Context &ctx, Cont &&cont) noexcept
        -> match_result<Iter>
        requires is_trivially_matchable<Inner>
        {
            for (std::size_t match_count = 0;; ++match_count, ++current)
            {
                if (auto rest_match = cont(current))
                    return rest_match;
                if (symbol::equals<Bound>(match_count) || current == end || !Inner::match_one(*current, ctx))
                    break;
            }
            return non_match(current);
        }

        template<std::forward_iterator Iter, match_continuation<Iter> Cont>
        static constexpr auto continue_unless_infinite_loop(Iter current, Cont &&cont) noexcept
        {
            return [=, &cont](Iter next) noexcept -> match_result<Iter> {
                if (will_loop_forever(current, next))
                    return non_match(current);
                return cont(next);
            };
        }

        template<std::forward_iterator Iter>
        static constexpr bool will_loop_forever(Iter /*current*/, Iter /*next*/) noexcept
        {
            return false;
        }

        template<std::forward_iterator Iter>
        static constexpr bool will_loop_forever(Iter current, Iter next) noexcept
        requires (symbol::is_infinity<UpperBound> && is_zero_length_matcher<Inner>)
        {
            return current == next;
        }
    };
}
#endif //MREGEX_NODES_REPETITION_HPP