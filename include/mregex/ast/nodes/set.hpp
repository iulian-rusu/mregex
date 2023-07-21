#ifndef MREGEX_NODES_SET_HPP
#define MREGEX_NODES_SET_HPP

#include <mregex/ast/nodes/trivially_matchable.hpp>
#include <mregex/ast/traits.hpp>

namespace meta::ast
{
    template<typename... Nodes>
    struct set : trivially_matchable<set<Nodes ...>>
    {
        static_assert(are_trivially_matchable<Nodes ...>, "only trivially matchable AST nodes can form a set");

        template<typename Context>
        static constexpr bool match_one(char input, Context &ctx) noexcept
        {
            return (Nodes::match_one(input, ctx) || ...);
        }
    };
}
#endif //MREGEX_NODES_SET_HPP