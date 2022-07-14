#ifndef MREGEX_NODES_OPTIONAL_HPP
#define MREGEX_NODES_OPTIONAL_HPP

#include <mregex/ast/astfwd.hpp>
#include <mregex/ast/ast_traits.hpp>
#include <mregex/ast/match_result.hpp>
#include <mregex/regex_context.hpp>

namespace meta::ast
{
    template<match_mode Mode, typename Inner>
    struct basic_optional
    {
        static constexpr std::size_t capture_count = Inner::capture_count;

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            if constexpr ((Mode == match_mode::greedy) ^ flags_of<Context>::ungreedy)
                return greedy_match(begin, end, it, ctx, cont);
            else
                return lazy_match(begin, end, it, ctx, cont);
        }

    private:
        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto greedy_match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            if (auto inner_match = Inner::match(begin, end, it, ctx, cont))
                return inner_match;
            return cont(it);
        }

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto lazy_match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            if (auto rest_match = cont(it))
                return rest_match;
            return Inner::match(begin, end, it, ctx, cont);
        }
    };
}
#endif //MREGEX_NODES_OPTIONAL_HPP