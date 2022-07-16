#ifndef MREGEX_NODES_IGNORE_PREFIX_HPP
#define MREGEX_NODES_IGNORE_PREFIX_HPP

#include <mregex/ast/nodes/terminals/terminal.hpp>
#include <mregex/ast/match_result.hpp>
#include <mregex/regex_context.hpp>

namespace meta::ast
{
    struct ignore_prefix : terminal
    {
        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            for (;; ++it)
            {
                if (auto rest_match = cont(it))
                {
                    std::get<0>(ctx.captures) = regex_capture_view<Iter>{it, rest_match.end};
                    return rest_match;
                }
                if (it == end)
                    break;
            }
            return {it, false};
        }
    };
}
#endif //MREGEX_NODES_IGNORE_PREFIX_HPP
