#ifndef MREGEX_NODES_WILDCARD_HPP
#define MREGEX_NODES_WILDCARD_HPP

#include <mregex/ast/astfwd.hpp>
#include <mregex/ast/nodes/set.hpp>

namespace meta::ast
{
    struct wildcard : trivially_matchable<wildcard>
    {
        template<typename Context>
        static constexpr bool match_one(char input, Context &ctx) noexcept
        {
            if constexpr (Context::flags::dotall)
                return true;
            return !linebreak::match_one(input, ctx);
        }
    };
}
#endif //MREGEX_NODES_WILDCARD_HPP