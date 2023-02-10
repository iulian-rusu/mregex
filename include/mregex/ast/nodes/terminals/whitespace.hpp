#ifndef MREGEX_NODES_WHITESPACE_HPP
#define MREGEX_NODES_WHITESPACE_HPP

#include <mregex/ast/nodes/terminals/trivially_matchable.hpp>
#include <mregex/ast/match_result.hpp>

namespace meta::ast
{
    struct whitespace : trivially_matchable<whitespace>
    {
        template<std::forward_iterator Iter, typename Context>
        static constexpr bool match_one(Iter current, Context &) noexcept
        {
            auto ch = *current;
            return ch == ' ' || ch == '\t' || ch == '\v' ||
                   ch == '\n' || ch == '\r' || ch == '\f';
        }
    };
}
#endif //MREGEX_NODES_WHITESPACE_HPP