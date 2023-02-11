#ifndef MREGEX_NODES_RANGE_HPP
#define MREGEX_NODES_RANGE_HPP

#include <mregex/ast/nodes/terminals/trivially_matchable.hpp>
#include <mregex/ast/match_result.hpp>
#include <mregex/regex_context.hpp>

namespace meta::ast
{
    template<char A, char B>
    struct range : trivially_matchable<range<A, B>>
    {
        static_assert(A < B, "invalid range bounds");

        template<std::forward_iterator Iter, typename Context>
        static constexpr bool match_one(Iter it, Context &) noexcept
        {
            auto input = *it;
            bool result = A <= input && input <= B;
            if constexpr (Context::flags::icase)
            {
                if (is_alpha(input))
                {
                    input ^= 0x20;
                    result |= A <= input && input <= B;
                }
            }
            return result;
        }
    };

    template<char C>
    struct range<C, C> : literal<C> {};
}
#endif //MREGEX_NODES_RANGE_HPP