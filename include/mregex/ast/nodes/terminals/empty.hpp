#ifndef MREGEX_NODES_EMPTY_HPP
#define MREGEX_NODES_EMPTY_HPP

#include <mregex/ast/nodes/terminals/terminal.hpp>
#include <mregex/ast/match_result.hpp>
#include <mregex/ast/traits.hpp>

namespace meta::ast
{
    struct empty : terminal, zero_length_matcher
    {
        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter /*begin*/, Iter /*end*/, Iter current, Context &, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            return cont(current);
        }
    };
}
#endif //MREGEX_NODES_EMPTY_HPP