#ifndef MREGEX_NODES_TRIVIALLY_MATCHABLE_HPP
#define MREGEX_NODES_TRIVIALLY_MATCHABLE_HPP

#include <mregex/ast/nodes/terminals/terminal.hpp>
#include <mregex/ast/match_result.hpp>

namespace meta::ast
{
    /**
     * Base type for trivially matchable AST nodes.
     * Trivially matchable nodes are terminals that can only match one input character at a time.
     *
     * @tparam Node The trivially matchable AST node derived from this type
     */
    template<typename Node>
    struct trivially_matchable : terminal
    {
        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter /*begin*/, Iter end, Iter current, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            if (current == end || !Node::match_one(current, ctx))
                return {current, false};
            return cont(std::next(current));
        }
    };
}
#endif //MREGEX_NODES_TRIVIALLY_MATCHABLE_HPP