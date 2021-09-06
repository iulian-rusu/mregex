#ifndef META_SEQUENCE_HPP
#define META_SEQUENCE_HPP

#include "astfwd.hpp"
#include "ast_traits.hpp"
#include "match_result.hpp"
#include "../regex_context.hpp"

namespace meta::ast
{
    template<typename First, typename... Rest>
    struct sequence
    {
        static constexpr std::size_t capture_count = capture_count_v<First, Rest ...>;
        static constexpr std::size_t sequence_size = 1 + sizeof... (Rest);

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires (!is_trivially_matchable_v<First>)
        {
            auto continuation = [=, &ctx, &cont](Iter new_it) noexcept {
                return sequence<Rest ...>::match(begin, end, new_it, ctx, cont);
            };
            return First::match(begin, end, it, ctx, continuation);
        }

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires (is_trivially_matchable_v<First> && !are_trivially_matchable_v<Rest ...>)
        {
            if (it == end)
                return {it, false};

            if (First::consume_one(it, ctx))
                return sequence<Rest ...>::match(begin, end, ++it, ctx, cont);
            return {it, false};
        }

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires are_trivially_matchable_v<First, Rest ...>
        {
            std::size_t const remaining_length = std::distance(it, end);
            if (remaining_length < sequence_size)
                return {it, false};
            return expand_trivial_match(it, ctx, cont, std::make_index_sequence<sequence_size>{});
        }

    private:
        template<std::forward_iterator Iter, typename Context, typename Continuation, std::size_t Index, std::size_t... Indices>
        static constexpr auto expand_trivial_match(
                Iter it,
                Context &ctx,
                Continuation &&cont,
                std::index_sequence<Index, Indices ...> &&
        ) noexcept -> match_result<Iter>
        {
            if (First::consume_one(it, ctx) && (Rest::consume_one(it + Indices, ctx) && ...))
                return cont(it + sequence_size);
            return {it, false};
        }
    };

    template<typename First>
    struct sequence<First> : First {};
}
#endif //META_SEQUENCE_HPP