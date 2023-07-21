#ifndef MREGEX_NODES_LITERAL_HPP
#define MREGEX_NODES_LITERAL_HPP

#include <mregex/ast/nodes/trivially_matchable.hpp>
#include <mregex/utility/char_traits.hpp>

namespace meta::ast
{
    template<char C>
    struct literal : trivially_matchable<literal<C>>
    {
        template<typename Context>
        static constexpr bool match_one(char input, Context &) noexcept
        {
            bool result = C == input;
            if constexpr (Context::flags::icase && is_alpha(C))
                result = result || flip_lowercase_bit(C) == input;
            return result;
        }
    };
}
#endif //MREGEX_NODES_LITERAL_HPP