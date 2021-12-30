#ifndef MREGEX_DISJUNCTION_HPP
#define MREGEX_DISJUNCTION_HPP

#include <mregex/ast/astfwd.hpp>
#include <mregex/ast/match_result.hpp>

namespace meta::ast
{
    template<typename First, typename... Rest>
    struct disjunction
    {
        static constexpr std::size_t capture_count = max_capture_count_v<First, Rest ...>;

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            if (auto first_match = First::match(begin, end, it, ctx, cont))
                return first_match;
            if constexpr (capture_count > 0)
                ctx.clear();
            return disjunction<Rest ...>::match(begin, end, it, ctx, cont);
        }
    };

    template<typename First>
    struct disjunction<First> : First {};
}
#endif //MREGEX_DISJUNCTION_HPP