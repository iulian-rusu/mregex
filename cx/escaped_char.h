#ifndef CX_ESCAPED_CHAR_H
#define CX_ESCAPED_CHAR_H

#include "utility/stack.h"
#include "symbol.h"
#include "astfwd.h"

namespace cx
{
    template<auto C>
    struct rule_for_escaped_char
    {
        using type =
                stack
                <
                    character<C>,
                    symbol::make_char
                >;
    };

    template<>
    struct rule_for_escaped_char<'n'>
    {
        using type =
                stack
                <
                    character<'n'>,
                    symbol::make_newline
                >;
    };

    template<>
    struct rule_for_escaped_char<'a'>
    {
        using type =
                stack
                <
                    character<'a'>,
                    symbol::make_alpha
                >;
    };

    template<>
    struct rule_for_escaped_char<'A'>
    {
        using type =
                stack
                <
                    character<'A'>,
                    symbol::make_alpha,
                    symbol::make_negated
                >;
    };

    template<>
    struct rule_for_escaped_char<'d'>
    {
        using type =
                stack
                <
                    character<'d'>,
                    symbol::make_digit
                >;
    };

    template<>
    struct rule_for_escaped_char<'D'>
    {
        using type =
                stack
                <
                    character<'D'>,
                    symbol::make_digit,
                    symbol::make_negated
                >;
    };
    template<>
    struct rule_for_escaped_char<'w'>
    {
        using type =
                stack
                <
                    character<'w'>,
                    symbol::make_word
                >;
    };

    template<>
    struct rule_for_escaped_char<'W'>
    {
        using type =
                stack
                <
                    character<'W'>,
                    symbol::make_word,
                    symbol::make_negated
                >;
    };

    template<>
    struct rule_for_escaped_char<'s'>
    {
        using type =
                stack
                <
                    character<'s'>,
                    symbol::make_whitespace
                >;
    };

    template<>
    struct rule_for_escaped_char<'S'>
    {
        using type =
                stack
                <
                    character<'S'>,
                    symbol::make_whitespace,
                    symbol::make_negated
                >;
    };

    template<>
    struct rule_for_escaped_char<'l'>
    {
        using type =
                stack
                <
                    character<'l'>,
                    symbol::make_lower
                >;
    };

    template<>
    struct rule_for_escaped_char<'L'>
    {
        using type =
                stack
                <
                    character<'L'>,
                    symbol::make_lower,
                    symbol::make_negated
                >;
    };

    template<>
    struct rule_for_escaped_char<'u'>
    {
        using type =
                stack
                <
                    character<'u'>,
                    symbol::make_upper
                >;
    };

    template<>
    struct rule_for_escaped_char<'U'>
    {
        using type =
                stack
                <
                    character<'U'>,
                    symbol::make_upper,
                    symbol::make_negated
                >;
    };

    template<>
    struct rule_for_escaped_char<'h'>
    {
        using type =
                stack
                <
                    character<'h'>,
                    symbol::make_hexa
                >;
    };

    template<>
    struct rule_for_escaped_char<'H'>
    {
        using type =
                stack
                <
                    character<'H'>,
                    symbol::make_hexa,
                    symbol::make_negated
                >;
    };

    template<auto C>
    using rule_for_escaped_char_t = typename rule_for_escaped_char<C>::type;
}
#endif //CX_ESCAPED_CHAR_H