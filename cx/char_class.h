#ifndef CX_CHAR_CLASS_H
#define CX_CHAR_CLASS_H

#include <type_traits>
#include "stack.h"
#include "symbol.h"
#include "ast.h"

/**
 * File defining rules for special character classes recognized by the grammar
 */
namespace cx
{

    template<auto C>
    struct rule_for_char_class
    {
        using type = stack<
                character<C>,
                symbol::make_char>;
    };

    template<>
    struct rule_for_char_class<'a'>
    {
        using type = stack<
                character<'a'>,
                symbol::make_alnum>;
    };

    template<>
    struct rule_for_char_class<'A'>
    {
        using type = stack<
                character<'A'>,
                symbol::make_alnum,
                symbol::make_negated>;
    };

    template<>
    struct rule_for_char_class<'d'>
    {
        using type = stack<
                character<'d'>,
                symbol::make_digit>;
    };

    template<>
    struct rule_for_char_class<'D'>
    {
        using type = stack<
                character<'D'>,
                symbol::make_digit,
                symbol::make_negated>;
    };
    template<>
    struct rule_for_char_class<'w'>
    {
        using type = stack<
                character<'w'>,
                symbol::make_word>;
    };

    template<>
    struct rule_for_char_class<'W'>
    {
        using type = stack<
                character<'W'>,
                symbol::make_word,
                symbol::make_negated>;
    };

    template<>
    struct rule_for_char_class<'s'>
    {
        using type = stack<
                character<'s'>,
                symbol::make_whitespace>;
    };

    template<>
    struct rule_for_char_class<'S'>
    {
        using type = stack<
                character<'S'>,
                symbol::make_whitespace,
                symbol::make_negated>;
    };

    template<>
    struct rule_for_char_class<'l'>
    {
        using type = stack<
                character<'l'>,
                symbol::make_lower>;
    };

    template<>
    struct rule_for_char_class<'L'>
    {
        using type = stack<
                character<'L'>,
                symbol::make_lower,
                symbol::make_negated>;
    };

    template<>
    struct rule_for_char_class<'u'>
    {
        using type = stack<
                character<'u'>,
                symbol::make_upper>;
    };

    template<>
    struct rule_for_char_class<'U'>
    {
        using type = stack<
                character<'U'>,
                symbol::make_upper,
                symbol::make_negated>;
    };

    template<>
    struct rule_for_char_class<'h'>
    {
        using type = stack<
                character<'h'>,
                symbol::make_hexa>;
    };

    template<>
    struct rule_for_char_class<'H'>
    {
        using type = stack<
                character<'H'>,
                symbol::make_hexa,
                symbol::make_negated>;
    };

    template<auto C>
    using rule_for_char_class_t = typename rule_for_char_class<C>::type;
}
#endif //CX_CHAR_CLASS_H
