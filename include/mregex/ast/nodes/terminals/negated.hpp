#ifndef MREGEX_NODES_NEGATED_HPP
#define MREGEX_NODES_NEGATED_HPP

#include <mregex/ast/nodes/terminals/terminal.hpp>
#include <mregex/ast/match_result.hpp>

namespace meta::ast
{
    template<typename Inner>
    struct negated : terminal
    {
        static_assert(is_trivially_matchable<Inner>, "only trivially matchable AST nodes can be negated");

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            if (it == end)
                return {it, false};
            if (match_one(it, ctx))
                return cont(std::next(it));
            return {it, false};
        }

        template<std::forward_iterator Iter, typename Context>
        static constexpr bool match_one(Iter current, Context &ctx) noexcept
        {
            return !Inner::match_one(current, ctx);
        }
    };
}
#endif //MREGEX_NODES_NEGATED_HPP