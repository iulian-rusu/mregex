#ifndef MREGEX_CHAR_CLASS_RULES_HPP
#define MREGEX_CHAR_CLASS_RULES_HPP

#include <mregex/grammar/grammar_actions.hpp>
#include <mregex/utility/type_sequence.hpp>

namespace meta::grammar
{
    /**
     * Metafunction that handles escaped sequences that are
     * recognized as a regex character class (\d, \w).
     *
     * @tparam C    The current character in the input pattern
     */
    template<char C>
    struct char_class_rule
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_literal
                >;
    };

    template<>
    struct char_class_rule<'d'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_digit
                >;
    };

    template<>
    struct char_class_rule<'D'>
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
    struct char_class_rule<'w'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_word
                >;
    };

    template<>
    struct char_class_rule<'W'>
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
    struct char_class_rule<'s'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_whitespace
                >;
    };

    template<>
    struct char_class_rule<'S'>
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
    struct char_class_rule<'l'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_lower
                >;
    };

    template<>
    struct char_class_rule<'L'>
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
    struct char_class_rule<'u'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_upper
                >;
    };

    template<>
    struct char_class_rule<'U'>
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
    struct char_class_rule<'x'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_hexa
                >;
    };

    template<>
    struct char_class_rule<'X'>
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
    struct char_class_rule<'R'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_linebreak
                >;
    };

    template<>
    struct char_class_rule<'N'>
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
    struct char_class_rule<'b'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_word_boundary
                >;
    };

    template<>
    struct char_class_rule<'B'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_word_boundary,
                    symbol::make_negated
                >;
    };

    template<>
    struct char_class_rule<'A'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_beginning_of_input
                >;
    };

    template<>
    struct char_class_rule<'Z'>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_end_of_input
                >;
    };
}
#endif //MREGEX_CHAR_CLASS_RULES_HPP