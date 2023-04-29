#ifndef MREGEX_NODES_TRIVIALLY_MATCHABLE_HPP
#define MREGEX_NODES_TRIVIALLY_MATCHABLE_HPP

#include <mregex/ast/match_result.hpp>
#include <mregex/utility/continuations.hpp>

namespace meta::ast
{
    /**
     * Base type for trivially matchable AST nodes.
     * Trivially matchable nodes can only match one input character at a time.
     *
     * @tparam Node The trivially matchable AST node derived from this type
     */
    template<typename Node>
    struct trivially_matchable
    {
        template<std::forward_iterator Iter, typename Context, match_continuation<Iter> Cont>
        static constexpr auto match(Iter /*begin*/, Iter end, Iter current, Context &ctx, Cont &&cont) noexcept
        -> match_result<Iter>
        {
            if (current != end && Node::match_one(current, ctx))
                return cont(std::next(current));
            return non_match(current);
        }
    };
}
#endif //MREGEX_NODES_TRIVIALLY_MATCHABLE_HPP