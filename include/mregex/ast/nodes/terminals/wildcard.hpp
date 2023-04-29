#ifndef MREGEX_NODES_WILDCARD_HPP
#define MREGEX_NODES_WILDCARD_HPP

#include <mregex/ast/nodes/trivially_matchable.hpp>

namespace meta::ast
{
    struct wildcard : trivially_matchable<wildcard>
    {
        template<std::forward_iterator Iter, typename Context>
        static constexpr bool match_one(Iter current, Context &ctx) noexcept
        {
            if constexpr (Context::flags::dotall)
                return true;
            return !linebreak::match_one(current, ctx);
        }
    };
}
#endif //MREGEX_NODES_WILDCARD_HPP