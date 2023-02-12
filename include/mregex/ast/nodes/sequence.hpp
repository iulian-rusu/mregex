#ifndef MREGEX_NODES_SEQUENCE_HPP
#define MREGEX_NODES_SEQUENCE_HPP

#include <mregex/ast/astfwd.hpp>
#include <mregex/ast/match_result.hpp>
#include <mregex/ast/traits.hpp>
#include <mregex/utility/distance.hpp>

namespace meta::ast
{
    template<typename First, typename... Rest>
    struct sequence
    {
        static constexpr std::size_t capture_count = count_captures<First, Rest ...>;
        static constexpr std::size_t size = 1 + sizeof... (Rest);

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            auto continuation = [=, &ctx, &cont](Iter new_it) noexcept {
                return sequence<Rest ...>::match(begin, end, new_it, ctx, cont);
            };
            return First::match(begin, end, it, ctx, continuation);
        }

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires (is_trivially_matchable<First> && !are_trivially_matchable<Rest ...>)
        {
            if (it == end || !First::match_one(it, ctx))
                return {it, false};
            return sequence<Rest ...>::match(begin, end, std::next(it), ctx, cont);
        }

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires are_trivially_matchable<First, Rest ...>
        {
            if (distance_less_than<size>(it, end))
                return {it, false};
            if constexpr (std::random_access_iterator<Iter>)
                return unrolled_trivial_match(it, ctx, cont, std::make_index_sequence<size - 1>{});
            else
                return unrolled_trivial_match(it, ctx, cont);
        }

    private:
        template<std::random_access_iterator Iter, typename Context, typename Continuation, std::size_t... Indices>
        static constexpr auto unrolled_trivial_match(
                Iter it, Context &ctx, Continuation &&cont,
                std::index_sequence<Indices ...> &&
        ) noexcept -> match_result<Iter>
        {
            if (First::match_one(it, ctx) && (Rest::match_one(std::next(it, Indices + 1), ctx) && ...))
                return cont(std::next(it, size));
            return {it, false};
        }

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto unrolled_trivial_match(Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            if (First::match_one(it, ctx) && (Rest::match_one(++it, ctx) && ...))
                return cont(std::next(it));
            return {it, false};
        }
    };

    template<typename Inner>
    struct sequence<Inner> : Inner {};
}
#endif //MREGEX_NODES_SEQUENCE_HPP