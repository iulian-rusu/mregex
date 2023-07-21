#ifndef MREGEX_NODES_SEQUENCE_HPP
#define MREGEX_NODES_SEQUENCE_HPP

#include <mregex/ast/astfwd.hpp>
#include <mregex/ast/match_result.hpp>
#include <mregex/ast/traits.hpp>
#include <mregex/utility/continuations.hpp>
#include <mregex/utility/distance.hpp>

namespace meta::ast
{
    template<typename First, typename... Rest>
    struct sequence
    {
        static constexpr std::size_t size = 1 + sizeof...(Rest);

        template<std::forward_iterator Iter, typename Context, match_continuation<Iter> Cont>
        static constexpr auto match(Iter begin, Iter end, Iter current, Context &ctx, Cont &&cont) noexcept
        -> match_result<Iter>
        {
            auto continuation = [=, &ctx, &cont](Iter next) noexcept -> match_result<Iter> {
                return sequence<Rest ...>::match(begin, end, next, ctx, cont);
            };
            return First::match(begin, end, current, ctx, continuation);
        }

        template<std::forward_iterator Iter, typename Context, match_continuation<Iter> Cont>
        static constexpr auto match(Iter /*begin*/, Iter end, Iter current, Context &ctx, Cont &&cont) noexcept
        -> match_result<Iter>
        requires are_trivially_matchable<First, Rest ...>
        {
            if (distance_less_than<size>(current, end))
                return non_match(current);
            if constexpr (std::random_access_iterator<Iter>)
                return unrolled_trivial_match(current, ctx, cont, std::make_index_sequence<size>{});
            else
                return unrolled_trivial_match(current, ctx, cont);
        }

    private:
        template<std::random_access_iterator Iter, typename Context, match_continuation<Iter> Cont, std::size_t... Indices>
        static constexpr auto unrolled_trivial_match(
                Iter current, Context &ctx, Cont &&cont,
                std::index_sequence<0, Indices ...>
        ) noexcept -> match_result<Iter>
        {
            if (First::match_one(*current, ctx) && (Rest::match_one(*std::next(current, Indices), ctx) && ...))
                return cont(std::next(current, size));
            return non_match(current);
        }

        template<std::forward_iterator Iter, typename Context, match_continuation<Iter> Cont>
        static constexpr auto unrolled_trivial_match(Iter current, Context &ctx, Cont &&cont) noexcept
        -> match_result<Iter>
        {
            if (First::match_one(*current, ctx) && (Rest::match_one(*++current, ctx) && ...))
                return cont(std::next(current));
            return non_match(current);
        }
    };

    template<typename Inner>
    struct sequence<Inner> : Inner {};
}
#endif //MREGEX_NODES_SEQUENCE_HPP