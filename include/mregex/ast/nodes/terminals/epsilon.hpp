#ifndef MREGEX_NODES_EPSILON_HPP
#define MREGEX_NODES_EPSILON_HPP

#include <mregex/ast/nodes/terminals/terminal.hpp>
#include <mregex/ast/match_result.hpp>

namespace meta::ast
{
    struct epsilon : terminal
    {
        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter, Iter, Iter it, Context &, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            return cont(it);
        }
    };
}
#endif //MREGEX_NODES_EPSILON_HPP