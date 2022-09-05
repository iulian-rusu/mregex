#ifndef MREGEX_GRAMMAR_ESCAPES_HPP
#define MREGEX_GRAMMAR_ESCAPES_HPP

#include <mregex/grammar/backrefs.hpp>

namespace meta::grammar
{
    /**
     * Metafunction that handles any escape sequence that is not a backreference.
     *
     * @tparam C    The current character in the input pattern
     */
    template<char C>
    struct begin_generic_escape_sequence
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_literal
                >;
    };

    template<>
    struct begin_generic_escape_sequence<'0'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::push_literal<'\0'>
                >;
    };


    template<>
    struct begin_generic_escape_sequence<'n'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::push_literal<'\n'>
                >;
    };

    template<>
    struct begin_generic_escape_sequence<'r'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::push_literal<'\r'>
                >;
    };

    template<>
    struct begin_generic_escape_sequence<'t'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::push_literal<'\t'>
                >;
    };

    template<>
    struct begin_generic_escape_sequence<'f'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::push_literal<'\f'>
                >;
    };

    template<>
    struct begin_generic_escape_sequence<'v'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::push_literal<'\v'>
                >;
    };

    template<>
    struct begin_generic_escape_sequence<'a'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::push_literal<'\a'>
                >;
    };

    template<>
    struct begin_generic_escape_sequence<'e'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::push_literal<'\x1b'>
                >;
    };

    template<>
    struct begin_generic_escape_sequence<'d'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_digit
                >;
    };

    template<>
    struct begin_generic_escape_sequence<'D'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_digit,
                    symbol::make_negated
                >;
    };
    template<>
    struct begin_generic_escape_sequence<'w'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_word
                >;
    };

    template<>
    struct begin_generic_escape_sequence<'W'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_word,
                    symbol::make_negated
                >;
    };

    template<>
    struct begin_generic_escape_sequence<'s'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_whitespace
                >;
    };

    template<>
    struct begin_generic_escape_sequence<'S'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_whitespace,
                    symbol::make_negated
                >;
    };

    template<>
    struct begin_generic_escape_sequence<'l'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_lower
                >;
    };

    template<>
    struct begin_generic_escape_sequence<'L'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_lower,
                    symbol::make_negated
                >;
    };

    template<>
    struct begin_generic_escape_sequence<'u'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_upper
                >;
    };

    template<>
    struct begin_generic_escape_sequence<'U'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_upper,
                    symbol::make_negated
                >;
    };

    template<>
    struct begin_generic_escape_sequence<'x'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_hexa
                >;
    };

    template<>
    struct begin_generic_escape_sequence<'X'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_hexa,
                    symbol::make_negated
                >;
    };

    template<>
    struct begin_generic_escape_sequence<'R'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_linebreak
                >;
    };

    template<>
    struct begin_generic_escape_sequence<'N'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_linebreak,
                    symbol::make_negated
                >;
    };

    template<>
    struct begin_generic_escape_sequence<'b'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_word_boundary,
                    symbol::unquantifiable
                >;
    };

    template<>
    struct begin_generic_escape_sequence<'B'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_word_boundary,
                    symbol::make_negated,
                    symbol::unquantifiable
                >;
    };

    template<>
    struct begin_generic_escape_sequence<'A'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_beginning_of_input,
                    symbol::unquantifiable
                >;
    };

    template<>
    struct begin_generic_escape_sequence<'Z'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_end_of_input,
                    symbol::unquantifiable
                >;
    };

    /**
     * Metafunction that handles an escaped character sequence.
     * Such sequences can be parsed either as a backreference or as a special escaped character.
     *
     * @tparam C    The current character in the input pattern
     */
    template<char C, bool = C != '0' && is_numeric_v<C>>
    struct begin_escape_sequence : begin_backref<C> {};

    template<char C>
    using begin_escape_sequence_t = typename begin_escape_sequence<C>::type;

    template<>
    struct begin_escape_sequence<'k', false> : begin_named_backref {};

    template<char C>
    struct begin_escape_sequence<C, false> : begin_generic_escape_sequence<C> {};

    /**
     * Metafunction that handles an escaped character sequence inside a set.
     * Sets do not allow anchors and backreferences. All such escape sequences are treaded as
     * literal characters.
     *
     * @tparam C    The current character in the input pattern
     */
    template<char C>
    struct begin_set_escape_sequence : begin_generic_escape_sequence<C> {};

    template<char C>
    using begin_set_escape_sequence_t = typename begin_set_escape_sequence<C>::type;

    template<>
    struct begin_set_escape_sequence<'A'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_literal
                >;
    };

    template<>
    struct begin_set_escape_sequence<'Z'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_literal
                >;
    };

    template<>
    struct begin_set_escape_sequence<'B'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_literal
                >;
    };

    template<>
    struct begin_set_escape_sequence<'b'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::push_literal<'\b'>
                >;
    };

    /**
     * Metafunction that handles an escaped character sequence inside a set range.
     * Escape sequences which can match multiple characters are invalid range bounds.
     *
     * @tparam C    The current character in the input pattern
     */
    template<char C>
    struct begin_set_range_escape_sequence : begin_set_escape_sequence<C> {};

    template<char C>
    using begin_set_range_escape_sequence_t = typename begin_set_range_escape_sequence<C>::type;

    // Special character classes which cannot be a range bound
    template<>
    struct begin_set_range_escape_sequence<'d'>
    {
        using type = reject;
    };

    template<>
    struct begin_set_range_escape_sequence<'D'>
    {
        using type = reject;
    };

    template<>
    struct begin_set_range_escape_sequence<'w'>
    {
        using type = reject;
    };

    template<>
    struct begin_set_range_escape_sequence<'W'>
    {
        using type = reject;
    };

    template<>
    struct begin_set_range_escape_sequence<'s'>
    {
        using type = reject;
    };

    template<>
    struct begin_set_range_escape_sequence<'S'>
    {
        using type = reject;
    };

    template<>
    struct begin_set_range_escape_sequence<'l'>
    {
        using type = reject;
    };

    template<>
    struct begin_set_range_escape_sequence<'L'>
    {
        using type = reject;
    };

    template<>
    struct begin_set_range_escape_sequence<'u'>
    {
        using type = reject;
    };

    template<>
    struct begin_set_range_escape_sequence<'U'>
    {
        using type = reject;
    };

    template<>
    struct begin_set_range_escape_sequence<'x'>
    {
        using type = reject;
    };

    template<>
    struct begin_set_range_escape_sequence<'X'>
    {
        using type = reject;
    };

    template<>
    struct begin_set_range_escape_sequence<'R'>
    {
        using type = reject;
    };

    template<>
    struct begin_set_range_escape_sequence<'N'>
    {
        using type = reject;
    };
}
#endif //MREGEX_GRAMMAR_ESCAPES_HPP