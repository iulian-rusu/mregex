#ifndef MREGEX_NODES_WILDCARD_HPP
#define MREGEX_NODES_WILDCARD_HPP

#include <mregex/ast/nodes/terminals/trivially_matchable.hpp>
#include <mregex/ast/match_result.hpp>
#include <mregex/regex_context.hpp>

namespace meta::ast
{
    struct wildcard : trivially_matchable<wildcard>
    {
        template<std::forward_iterator Iter, typename Context>
        static constexpr bool match_one(Iter current, Context &) noexcept
        {
            if constexpr (Context::flags::dotall)
                return true;
            auto ch = *current;
            return ch != '\n' && ch != '\r';
        }
    };
}
#endif //MREGEX_NODES_WILDCARD_HPP