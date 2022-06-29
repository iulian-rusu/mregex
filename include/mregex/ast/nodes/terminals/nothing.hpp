#ifndef MREGEX_NODES_NOTHING_HPP
#define MREGEX_NODES_NOTHING_HPP

#include <mregex/ast/nodes/terminals/terminal.hpp>
#include <mregex/ast/match_result.hpp>

namespace meta::ast
{
    struct nothing : terminal
    {
        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter, Iter, Iter it, Context &, Continuation &&) noexcept
        -> match_result<Iter>
        {
            return {it, false};
        }

        template<std::forward_iterator Iter, typename Context>
        static constexpr bool match_one(Iter, Context &) noexcept
        {
            return false;
        }
    };
}
#endif //MREGEX_NODES_NOTHING_HPP