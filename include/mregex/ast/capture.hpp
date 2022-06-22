#ifndef MREGEX_CAPTURE_HPP
#define MREGEX_CAPTURE_HPP

#include <mregex/regex_context.hpp>
#include <mregex/ast/astfwd.hpp>
#include <mregex/ast/ast_traits.hpp>
#include <mregex/ast/match_result.hpp>

namespace meta::ast
{
    template<std::size_t ID, typename Inner>
    struct capture
    {
        static constexpr std::size_t capture_count = 1 + Inner::capture_count;

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            auto continuation = [=, &ctx, &cont](Iter new_it) noexcept {
                std::get<ID>(ctx.captures) = regex_capture_view<ID, Iter>{it, new_it};
                return cont(new_it);
            };
            if (auto inner_match = Inner::match(begin, end, it, ctx, continuation))
                return inner_match;
            std::get<ID>(ctx.captures).clear();
            return {it, false};
        }
    };
}
#endif //MREGEX_CAPTURE_HPP