#ifndef MREGEX_NODES_EMPTY_HPP
#define MREGEX_NODES_EMPTY_HPP

#include <mregex/ast/astfwd.hpp>
#include <mregex/ast/match_result.hpp>
#include <mregex/ast/traits.hpp>
#include <mregex/utility/continuations.hpp>

namespace meta::ast
{
    struct empty : zero_length_matcher
    {
        template<std::forward_iterator Iter, typename Context, match_continuation<Iter> Cont>
        static constexpr auto match(Iter /*begin*/, Iter /*end*/, Iter current, Context &, Cont &&cont) noexcept
        -> match_result<Iter>
        {
            return cont(current);
        }
    };
}
#endif //MREGEX_NODES_EMPTY_HPP