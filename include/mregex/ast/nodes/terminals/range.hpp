#ifndef MREGEX_NODES_RANGE_HPP
#define MREGEX_NODES_RANGE_HPP

#include <mregex/ast/nodes/terminals/terminal.hpp>
#include <mregex/ast/match_result.hpp>
#include <mregex/regex_context.hpp>

namespace meta::ast
{
    template<char A, char B>
    struct range : terminal
    {
        static_assert(A < B, "invalid range bounds");

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
            auto ch = *current;
            bool result = A <= ch && ch <= B;
            if constexpr (flags_of<Context>::icase)
            {
                if (is_alpha(ch))
                {
                    ch ^= 0x20;
                    result |= A <= ch && ch <= B;
                }
            }
            return result;
        }
    };

    template<char C>
    struct range<C, C> : literal<C> {};
}
#endif //MREGEX_NODES_RANGE_HPP