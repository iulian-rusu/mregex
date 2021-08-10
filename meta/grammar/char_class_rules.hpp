#ifndef META_CHAR_CLASS_HPP
#define META_CHAR_CLASS_HPP

#include "grammar_actions.hpp"
#include "../utility/stack.hpp"
#include "../symbol.hpp"
#include "../ast/astfwd.hpp"

namespace meta::grammar
{
    template<auto C>
    struct char_class_rule
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_char
                >;
    };

    template<>
    struct char_class_rule<'a'>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_alpha
                >;
    };

    template<>
    struct char_class_rule<'A'>
    {
        using type =
                stack
                <
                    advance,
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
                    advance,
                    symbol::make_digit
                >;
    };

    template<>
    struct char_class_rule<'D'>
    {
        using type =
                stack
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
                stack
                <
                    advance,
                    symbol::make_word
                >;
    };

    template<>
    struct char_class_rule<'W'>
    {
        using type =
                stack
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
                stack
                <
                    advance,
                    symbol::make_whitespace
                >;
    };

    template<>
    struct char_class_rule<'S'>
    {
        using type =
                stack
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
                stack
                <
                    advance,
                    symbol::make_lower
                >;
    };

    template<>
    struct char_class_rule<'L'>
    {
        using type =
                stack
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
                stack
                <
                    advance,
                    symbol::make_upper
                >;
    };

    template<>
    struct char_class_rule<'U'>
    {
        using type =
                stack
                <
                    advance,
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
                    advance,
                    symbol::make_hexa
                >;
    };

    template<>
    struct char_class_rule<'H'>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_hexa,
                    symbol::make_negated
                >;
    };

    template<auto C>
    using char_class_rule_t = typename char_class_rule<C>::type;
}
#endif //META_CHAR_CLASS_HPP