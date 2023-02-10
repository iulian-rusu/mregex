#ifndef MREGEX_NODES_NOTHING_HPP
#define MREGEX_NODES_NOTHING_HPP

#include <mregex/ast/nodes/terminals/trivially_matchable.hpp>
#include <mregex/ast/match_result.hpp>

namespace meta::ast
{
    struct nothing : trivially_matchable<nothing>
    {
        template<std::forward_iterator Iter, typename Context>
        static constexpr bool match_one(Iter, Context &) noexcept
        {
            return false;
        }
    };
}
#endif //MREGEX_NODES_NOTHING_HPP