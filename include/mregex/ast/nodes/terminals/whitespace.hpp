#ifndef MREGEX_NODES_WHITESPACE_HPP
#define MREGEX_NODES_WHITESPACE_HPP

#include <mregex/ast/nodes/terminals/trivially_matchable.hpp>

namespace meta::ast
{
    struct whitespace : trivially_matchable<whitespace>
    {
        template<std::forward_iterator Iter, typename Context>
        static constexpr bool match_one(Iter current, Context &) noexcept
        {
            auto input = *current;
            return input == ' ' || input == '\t' || input == '\v' ||
                   input == '\n' || input == '\r' || input == '\f';
        }
    };
}
#endif //MREGEX_NODES_WHITESPACE_HPP