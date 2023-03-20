#ifndef MREGEX_NODES_NEGATED_HPP
#define MREGEX_NODES_NEGATED_HPP

#include <mregex/ast/nodes/terminals/trivially_matchable.hpp>

namespace meta::ast
{
    template<typename Inner>
    struct negated : trivially_matchable<negated<Inner>>
    {
        static_assert(is_trivially_matchable<Inner>, "only trivially matchable AST nodes can be negated");

        template<std::forward_iterator Iter, typename Context>
        static constexpr bool match_one(Iter current, Context &ctx) noexcept
        {
            return !Inner::match_one(current, ctx);
        }
    };
}
#endif //MREGEX_NODES_NEGATED_HPP