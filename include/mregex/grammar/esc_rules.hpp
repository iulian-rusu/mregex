#ifndef MREGEX_ESC_RULES_HPP
#define MREGEX_ESC_RULES_HPP

#include <mregex/grammar/char_class_rules.hpp>
#include <mregex/grammar/backref_rules.hpp>

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
                    symbol::push_literal<'\0'>
                >;
    };

    template<>
    struct esc_rule<'n', false>
    {
        using type =
                stack
                <
                    advance,
                    symbol::push_literal<'\n'>
                >;
    };

    template<>
    struct esc_rule<'r', false>
    {
        using type =
                stack
                <
                    advance,
                    symbol::push_literal<'\r'>
                >;
    };

    template<>
    struct esc_rule<'t', false>
    {
        using type =
                stack
                <
                    advance,
                    symbol::push_literal<'\t'>
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
#endif //MREGEX_ESC_RULES_HPP