#ifndef CX_CHAR_CLASS_H
#define CX_CHAR_CLASS_H

#include "utility/stack.h"
#include "symbol.h"
#include "astfwd.h"

namespace cx
{
    template<auto C>
    struct char_class_rule
    {
        using type =
                stack
                <
                    character<C>,
                    symbol::make_char
                >;
    };

    template<>
    struct char_class_rule<'a'>
    {
        using type =
                stack
                <
                    character<'a'>,
                    symbol::make_alpha
                >;
    };

    template<>
    struct char_class_rule<'A'>
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
    struct char_class_rule<'d'>
    {
        using type =
                stack
                <
                    character<'d'>,
                    symbol::make_digit
                >;
    };

    template<>
    struct char_class_rule<'D'>
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
    struct char_class_rule<'w'>
    {
        using type =
                stack
                <
                    character<'w'>,
                    symbol::make_word
                >;
    };

    template<>
    struct char_class_rule<'W'>
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
    struct char_class_rule<'s'>
    {
        using type =
                stack
                <
                    character<'s'>,
                    symbol::make_whitespace
                >;
    };

    template<>
    struct char_class_rule<'S'>
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
    struct char_class_rule<'l'>
    {
        using type =
                stack
                <
                    character<'l'>,
                    symbol::make_lower
                >;
    };

    template<>
    struct char_class_rule<'L'>
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
    struct char_class_rule<'u'>
    {
        using type =
                stack
                <
                    character<'u'>,
                    symbol::make_upper
                >;
    };

    template<>
    struct char_class_rule<'U'>
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
    struct char_class_rule<'h'>
    {
        using type =
                stack
                <
                    character<'h'>,
                    symbol::make_hexa
                >;
    };

    template<>
    struct char_class_rule<'H'>
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
    using char_class_rule_t = typename char_class_rule<C>::type;
}
#endif //CX_CHAR_CLASS_H