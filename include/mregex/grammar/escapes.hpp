#ifndef MREGEX_GRAMMAR_ESCAPES_HPP
#define MREGEX_GRAMMAR_ESCAPES_HPP

#include <mregex/grammar/backrefs.hpp>
#include <mregex/grammar/char_classes.hpp>

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
                type_sequence
                <
                    advance,
                    symbol::backref_id<C - '0'>
                >;
    };

    template<>
    struct esc_rule<'k', false>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::expect<'<'>,
                    symbol::backref_name_begin
                >;
    };

    template<>
    struct esc_rule<'0', false>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::push_literal<'\0'>
                >;
    };

    template<>
    struct esc_rule<'n', false>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::push_literal<'\n'>
                >;
    };

    template<>
    struct esc_rule<'r', false>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::push_literal<'\r'>
                >;
    };

    template<>
    struct esc_rule<'t', false>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::push_literal<'\t'>
                >;
    };

    template<>
    struct esc_rule<'f', false>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::push_literal<'\f'>
                >;
    };

    template<>
    struct esc_rule<'v', false>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::push_literal<'\v'>
                >;
    };

    template<>
    struct esc_rule<'a', false>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::push_literal<'\a'>
                >;
    };

    template<>
    struct esc_rule<'e', false>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::push_literal<0x1b>
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
#endif //MREGEX_GRAMMAR_ESCAPES_HPP