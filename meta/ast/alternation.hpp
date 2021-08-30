#ifndef META_ALTERNATION_HPP
#define META_ALTERNATION_HPP

#include "astfwd.hpp"
#include "ast_traits.hpp"
#include "match_result.hpp"

namespace meta::ast
{
    template<bool, typename First, typename... Rest>
    struct alternation_base
    {
        static constexpr std::size_t capture_count = capture_count_v<First, Rest ...>;

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            if (auto first_match = First::match(begin, end, it, ctx, cont))
                return first_match;
            return alternation<Rest ...>::match(begin, end, it, ctx, cont);
        }
    };

    template<typename First, typename... Rest>
    struct alternation_base<true, First, Rest ...> : set<First, Rest ...> {};

    template<typename First, typename... Rest>
    struct alternation : alternation_base<are_trivially_matchable_v<First, Rest ...>, First, Rest ...> {};

    template<typename First>
    struct alternation<First> : First {};
}
#endif //META_ALTERNATION_HPP
