#ifndef MREGEX_NODES_LITERAL_HPP
#define MREGEX_NODES_LITERAL_HPP

#include <mregex/ast/nodes/terminals/terminal.hpp>
#include <mregex/ast/match_result.hpp>
#include <mregex/utility/char_traits.hpp>
#include <mregex/regex_context.hpp>

namespace meta::ast
{
    template<char C>
    struct literal : terminal
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
            bool result = C == *current;
            if constexpr (flags_of<Context>::icase)
                result |= toggle_case<C> == *current;
            return result;
        }
    };
}
#endif //MREGEX_NODES_LITERAL_HPP