#ifndef MREGEX_ESC_RULES_HPP
#define MREGEX_ESC_RULES_HPP

#include <mregex/grammar/backref_rules.hpp>
#include <mregex/grammar/char_class_rules.hpp>

namespace meta::grammar
{
    /**
     * Metafunction that decides how to handle an escaped character
     * or sequence of characters.
     *
     * @tparam C    The current character in the input pattern
     */
    template<char C, bool = C != '0' && is_numeric_v<C>>
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
    struct esc_rule<'k', false>
    {
        using type =
                stack
                <
                    advance,
                    symbol::backref_name_expect
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

    template<>
    struct esc_rule<'f', false>
    {
        using type =
                stack
                <
                    advance,
                    symbol::push_literal<'\f'>
                >;
    };

    template<>
    struct esc_rule<'v', false>
    {
        using type =
                stack
                <
                    advance,
                    symbol::push_literal<'\v'>
                >;
    };

    /**
     * If none of the rules from above match, parse the escaped sequence
     * as a special character class.
     */
    template<char C>
    struct esc_rule<C, false> : char_class_rule<C> {};

    template<char C>
    using esc_rule_t = typename esc_rule<C>::type;
}
#endif //MREGEX_ESC_RULES_HPP