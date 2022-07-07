#ifndef MREGEX_NODES_ANCHORS_HPP
#define MREGEX_NODES_ANCHORS_HPP

#include <mregex/ast/nodes/terminals/terminal.hpp>
#include <mregex/ast/match_result.hpp>
#include <mregex/regex_context.hpp>

namespace meta::ast
{
    struct beginning : terminal
    {
        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            if (it == begin)
                return cont(it);
            if constexpr (flags_of<Context>::multiline)
                if (it != end && *it == '\n')
                    return cont(it);
            return {it, false};
        }
    };

    struct ending : terminal
    {
        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter, Iter end, Iter it, Context &, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            if (it == end)
                return cont(it);
            if constexpr (flags_of<Context>::multiline)
                if (*it == '\n')
                    return cont(it);
            return {it, false};
        }
    };
}
#endif //MREGEX_NODES_ANCHORS_HPP