#ifndef MREGEX_NODES_LITERAL_HPP
#define MREGEX_NODES_LITERAL_HPP

#include <mregex/ast/nodes/terminals/trivially_matchable.hpp>
#include <mregex/utility/char_traits.hpp>
#include <mregex/regex_context.hpp>

namespace meta::ast
{
    template<char C>
    struct literal : trivially_matchable<literal<C>>
    {
        template<std::forward_iterator Iter, typename Context>
        static constexpr bool match_one(Iter current, Context &) noexcept
        {
            bool result = C == *current;
            if constexpr (Context::flags::icase)
                result |= toggle_case<C> == *current;
            return result;
        }
    };
}
#endif //MREGEX_NODES_LITERAL_HPP