#ifndef META_ESC_SEQUENCE_HPP
#define META_ESC_SEQUENCE_HPP

#include "char_class_rules.hpp"
#include "backref_rules.hpp"
#include "../utility/stack.hpp"
#include "../symbol.hpp"
#include "../ast/astfwd.hpp"

namespace meta::grammar
{
    /**
     * Metafunction that decides how to handle an escaped character
     * or sequence of characters.
     *
     * @tparam C    The current character in the input pattern
     */
    template<auto C, bool = C != '0' && is_numeric_v<C>>
    struct esc_rule
    {
        using type =
                stack
                <
                    advance,
                    symbol::backref_id<C - '0'>
                >;
    };

    template<>
    struct esc_rule<'0', false>
    {
        using type =
                stack
                <
                    advance,
                    symbol::push_char<'\0'>
                >;
    };

    template<>
    struct esc_rule<'n', false>
    {
        using type =
                stack
                <
                    advance,
                    symbol::push_char<'\n'>
                >;
    };

    template<>
    struct esc_rule<'r', false>
    {
        using type =
                stack
                <
                    advance,
                    symbol::push_char<'\r'>
                >;
    };

    template<>
    struct esc_rule<'t', false>
    {
        using type =
                stack
                <
                    advance,
                    symbol::push_char<'\t'>
                >;
    };

    template<auto C>
    struct esc_rule<C, false>
    {
        using type = char_class_rule_t<C>;
    };

    template<auto C>
    using esc_rule_t = typename esc_rule<C>::type;
}
#endif //META_ESC_SEQUENCE_HPP