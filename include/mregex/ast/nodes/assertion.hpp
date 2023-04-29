#ifndef MREGEX_NODES_ASSERTION_HPP
#define MREGEX_NODES_ASSERTION_HPP

#include <mregex/ast/match_result.hpp>
#include <mregex/ast/traits.hpp>
#include <mregex/utility/continuations.hpp>

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
        using predicate_type = Predicate;

        template<std::forward_iterator Iter, typename Context, match_continuation<Iter> Cont>
        static constexpr auto match(Iter begin, Iter end, Iter current, Context &ctx, Cont &&cont) noexcept
        -> match_result<Iter>
        {
            if (Predicate::is_match(begin, end, current, ctx))
                return cont(current);
            return non_match(current);
        }
    };
}
#endif //MREGEX_NODES_ASSERTION_HPP