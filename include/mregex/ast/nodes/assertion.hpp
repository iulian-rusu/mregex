#ifndef MREGEX_NODES_ASSERTION_HPP
#define MREGEX_NODES_ASSERTION_HPP

#include <mregex/ast/match_result.hpp>
#include <mregex/ast/traits.hpp>

namespace meta::ast
{
    /**
     * Base type for AST nodes which indicate assertions.
     * Assertions are zero-length matchers that test the ability to match a pattern
     * without consuming any input characters.
     *
     * @tparam Predicate  A predicate type which is used to test for a match at a given position
     */
    template<typename Predicate>
    struct assertion : zero_length_matcher
    {
        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            if (Predicate::is_match(begin, end, it, ctx))
                return cont(it);
            return {it, false};
        }
    };
}
#endif //MREGEX_NODES_ASSERTION_HPP