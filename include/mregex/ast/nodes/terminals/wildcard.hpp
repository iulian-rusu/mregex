#ifndef MREGEX_NODES_WILDCARD_HPP
#define MREGEX_NODES_WILDCARD_HPP

#include <mregex/ast/nodes/terminals/terminal.hpp>
#include <mregex/ast/match_result.hpp>
#include <mregex/regex_context.hpp>

namespace meta::ast
{
    struct wildcard : terminal
    {
        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            if (it == end)
                return {it, false};
            if (match_one(it, ctx))
                return cont(std::next(it));
            return {it, false};
        }

        template<std::forward_iterator Iter, typename Context>
        static constexpr bool match_one(Iter current, Context &) noexcept
        {
            if constexpr (context_flags<Context>::dotall)
                return true;
            auto ch = *current;
            return ch != '\n' && ch != '\r';
        }
    };
}
#endif //MREGEX_NODES_WILDCARD_HPP