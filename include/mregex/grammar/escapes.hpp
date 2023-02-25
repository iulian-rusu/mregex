#ifndef MREGEX_GRAMMAR_ESCAPES_HPP
#define MREGEX_GRAMMAR_ESCAPES_HPP

#include <mregex/grammar/backrefs.hpp>

namespace meta::grammar
{
    /**
     * Metafunction that defines symbols used to handle a single escaped character.
     *
     * @tparam C    The current character in the input pattern
     */
    template<char C>
    struct handle_escaped_char
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_literal
                >;
    };

    template<>
    struct handle_escaped_char<'0'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::push_literal<'\0'>
                >;
    };


    template<>
    struct handle_escaped_char<'n'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::push_literal<'\n'>
                >;
    };

    template<>
    struct handle_escaped_char<'r'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::push_literal<'\r'>
                >;
    };

    template<>
    struct handle_escaped_char<'t'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::push_literal<'\t'>
                >;
    };

    template<>
    struct handle_escaped_char<'f'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::push_literal<'\f'>
                >;
    };

    template<>
    struct handle_escaped_char<'v'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::push_literal<'\v'>
                >;
    };

    template<>
    struct handle_escaped_char<'a'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::push_literal<'\a'>
                >;
    };

    template<>
    struct handle_escaped_char<'e'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::push_literal<'\x1b'>
                >;
    };

    template<>
    struct handle_escaped_char<'d'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_digit
                >;
    };

    template<>
    struct handle_escaped_char<'D'>
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
    struct handle_escaped_char<'w'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_word
                >;
    };

    template<>
    struct handle_escaped_char<'W'>
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
    struct handle_escaped_char<'s'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_whitespace
                >;
    };

    template<>
    struct handle_escaped_char<'S'>
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
    struct handle_escaped_char<'l'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_lower
                >;
    };

    template<>
    struct handle_escaped_char<'L'>
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
    struct handle_escaped_char<'u'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_upper
                >;
    };

    template<>
    struct handle_escaped_char<'U'>
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
    struct handle_escaped_char<'R'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_linebreak
                >;
    };

    template<>
    struct handle_escaped_char<'N'>
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
    struct handle_escaped_char<'b'>
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
    struct handle_escaped_char<'B'>
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
    struct handle_escaped_char<'A'>
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
    struct handle_escaped_char<'Z'>
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
     * Metafunction that defines symbols used to handle an escaped character sequence.
     * Such sequences can be parsed either as a backreference or as a special escape sequence.
     *
     * @tparam C    The current character in the input pattern
     */
    template<char C, bool = C != '0' && is_numeric(C)>
    struct handle_escape_sequence;

    template<char C>
    struct handle_escape_sequence<C, true> : begin_backref<C> {};

    template<>
    struct handle_escape_sequence<'k', false> : begin_named_backref {};

    template<char C>
    struct handle_escape_sequence<C, false> : handle_escaped_char<C> {};

    template<char C>
    using handle_escape_sequence_t = typename handle_escape_sequence<C>::type;

    /**
     * Metafunction that defines symbols used to handle an escaped character sequence inside a set.
     * Sets do not allow anchors and backreferences. All such escape sequences are treaded as
     * literal characters.
     *
     * @tparam C    The current character in the input pattern
     */
    template<char C>
    struct handle_set_escaped_char : handle_escaped_char<C> {};

    template<>
    struct handle_set_escaped_char<'A'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_literal
                >;
    };

    template<>
    struct handle_set_escaped_char<'Z'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_literal
                >;
    };

    template<>
    struct handle_set_escaped_char<'B'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_literal
                >;
    };

    template<>
    struct handle_set_escaped_char<'b'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::push_literal<'\b'>
                >;
    };

    template<char C>
    using handle_set_escaped_char_t = typename handle_set_escaped_char<C>::type;

    /**
     * Metafunction that defines symbols used to handle an escaped character sequence inside a set range.
     * Escape sequences which can match multiple characters are invalid range bounds.
     *
     * @tparam C    The current character in the input pattern
     */
    template<char C>
    struct handle_set_range_escaped_char : handle_set_escaped_char<C> {};

    // Special character classes which cannot be range bounds
    template<>
    struct handle_set_range_escaped_char<'d'>
    {
        using type = reject;
    };

    template<>
    struct handle_set_range_escaped_char<'D'>
    {
        using type = reject;
    };

    template<>
    struct handle_set_range_escaped_char<'w'>
    {
        using type = reject;
    };

    template<>
    struct handle_set_range_escaped_char<'W'>
    {
        using type = reject;
    };

    template<>
    struct handle_set_range_escaped_char<'s'>
    {
        using type = reject;
    };

    template<>
    struct handle_set_range_escaped_char<'S'>
    {
        using type = reject;
    };

    template<>
    struct handle_set_range_escaped_char<'l'>
    {
        using type = reject;
    };

    template<>
    struct handle_set_range_escaped_char<'L'>
    {
        using type = reject;
    };

    template<>
    struct handle_set_range_escaped_char<'u'>
    {
        using type = reject;
    };

    template<>
    struct handle_set_range_escaped_char<'U'>
    {
        using type = reject;
    };

    template<>
    struct handle_set_range_escaped_char<'R'>
    {
        using type = reject;
    };

    template<>
    struct handle_set_range_escaped_char<'N'>
    {
        using type = reject;
    };

    template<char C>
    using handle_set_range_escaped_char_t = typename handle_set_range_escaped_char<C>::type;
}
#endif //MREGEX_GRAMMAR_ESCAPES_HPP