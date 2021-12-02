#ifndef MREGEX_STAR_HPP
#define MREGEX_STAR_HPP

#include <mregex/ast/astfwd.hpp>
#include <mregex/ast/ast_traits.hpp>
#include <mregex/ast/match_result.hpp>

namespace meta::ast
{
    template<typename Inner>
    struct star
    {
        static constexpr std::size_t capture_count = Inner::capture_count;

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            if constexpr (flags<Context>::ungreedy)
                return lazy_match(begin, end, it, ctx, cont);
            else
                return greedy_match(begin, end, it, ctx, cont);
        }

    private:
        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto greedy_match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires (!is_trivially_matchable_v<Inner> || !std::bidirectional_iterator<Iter>)
        {
            auto continuation = [=, &ctx, &cont](Iter new_it) noexcept -> match_result<Iter> {
                if (new_it == it)
                    return {new_it, false};
                return greedy_match(begin, end, new_it, ctx, cont);
            };
            if (auto inner_match = Inner::match(begin, end, it, ctx, continuation))
                return inner_match;
            return cont(it);
        }

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto greedy_match(Iter, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires (is_trivially_matchable_v<Inner> && std::bidirectional_iterator<Iter>)
        {
            Iter start = it;
            for (; it != end; ++it)
            {
                if (!Inner::consume_one(it, ctx))
                    break;
            }
            for (; it != start; --it)
            {
                if (auto rest_match = cont(it))
                    return rest_match;
            }
            return cont(it);
        }

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto lazy_match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires (!is_trivially_matchable_v<Inner>)
        {
            if (auto rest_match = cont(it))
                return rest_match;

            auto continuation = [=, &ctx, &cont](Iter new_it) noexcept -> match_result<Iter> {
                if (new_it == it)
                    return {new_it, false};
                return lazy_match(begin, end, new_it, ctx, cont);
            };
            return Inner::match(begin, end, it, ctx, continuation);
        }

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto lazy_match(Iter, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires is_trivially_matchable_v<Inner>
        {
            for (; it != end; ++it)
            {
                if (auto rest_match = cont(it))
                    return rest_match;
                if (!Inner::consume_one(it, ctx))
                    break;
            }
            return cont(it);
        }
    };
}
#endif //MREGEX_STAR_HPP