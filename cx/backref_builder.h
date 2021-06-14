#ifndef CX_BACKREF_BUILDER_H
#define CX_BACKREF_BUILDER_H

#include <type_traits>
#include "stack.h"
#include "symbol.h"
#include "ast.h"

/**
 * File with metafunctions for building backreference nodes
 */
namespace cx
{
    /**
     * Type trait to check if a character is numeric at compile-time
     *
     * @tparam C    The character being checketd
     * @tparam B    Helper bool to test if the character is numeric
     */
    template<auto C, bool B = ('0' <= C && C <= '9')>
    struct is_numeric : std::bool_constant<B> {};

    template<auto C>
    constexpr bool is_numeric_v = is_numeric<C>::value;

    /**
     * Metafunction that decides using SFINAE if the current rule
     * is a backreference building rule or an escaped character class rule
     *
     * @tparam C    The current character being in the regex pattern
     */
    template<auto C, bool = (C != '0' && is_numeric_v<C>)>
    struct decide_esc_rule
    {
        using type = stack<
                character<C>,
                symbol::backref_id<C - '0'>>;
    };

    template<auto C>
    struct decide_esc_rule<C, false>
    {
        using type = rule_for_char_class_t<C>;
    };

    template<auto C>
    using decide_esc_rule_t = typename decide_esc_rule<C>::type;

    /**
     * Metafunction that decides using SFINAE if the current rule
     * will continue the backreference building or will finalize it
     *
     * @tparam C    The current character in the regex pattern
     * @tparam ID   The current backreference ID on the stack
     */
    template<auto C, std::size_t ID, bool = is_numeric_v<C>>
    struct continue_backref_id
    {
        using type = stack<
                character<C>,
                symbol::backref_id<10 * ID + C - '0'>>;
    };

    template<auto C, std::size_t ID>
    struct continue_backref_id<C, ID, false>
    {
        using type = symbol::make_backref<ID>;
    };

    template<auto C, std::size_t ID>
    using continue_backref_id_t = typename continue_backref_id<C, ID>::type;
}
#endif //CX_BACKREF_BUILDER_H
