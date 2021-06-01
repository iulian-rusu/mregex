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
    struct make_char_class
    {
        using type = symbol::make_char;
    };

    template<>
    struct make_char_class<'.'>
    {
        using type = symbol::make_wildcard;
    };

    template<>
    struct make_char_class<'^'>
    {
        using type = symbol::make_beginning;
    };

    template<>
    struct make_char_class<'$'>
    {
        using type = symbol::make_ending;
    };

    template<auto C>
    using make_char_class_t = typename make_char_class<C>::type;

    template<typename Symbol, typename Char>
    struct rule_for_escaped_char
    {
        using type = typename rule_for_escaped_char<symbol::esc, Char>::type;
    };

    template<auto C>
    struct rule_for_escaped_char<symbol::esc, character<C>>
    {
        using type = stack<
                character<C>,
                symbol::make_char>;
    };

    template<>
    struct rule_for_escaped_char<symbol::esc, character<'a'>>
    {
        using type = stack<
                character<'a'>,
                symbol::make_alnum>;
    };

    template<>
    struct rule_for_escaped_char<symbol::esc, character<'A'>>
    {
        using type = stack<
                character<'A'>,
                symbol::make_alnum,
                symbol::make_negated>;
    };

    template<>
    struct rule_for_escaped_char<symbol::esc, character<'d'>>
    {
        using type = stack<
                character<'d'>,
                symbol::make_digit>;
    };

    template<>
    struct rule_for_escaped_char<symbol::esc, character<'D'>>
    {
        using type = stack<
                character<'D'>,
                symbol::make_digit,
                symbol::make_negated>;
    };
    template<>
    struct rule_for_escaped_char<symbol::esc, character<'w'>>
    {
        using type = stack<
                character<'w'>,
                symbol::make_word>;
    };

    template<>
    struct rule_for_escaped_char<symbol::esc, character<'W'>>
    {
        using type = stack<
                character<'W'>,
                symbol::make_word,
                symbol::make_negated>;
    };

    template<>
    struct rule_for_escaped_char<symbol::esc, character<'s'>>
    {
        using type = stack<
                character<'s'>,
                symbol::make_whitespace>;
    };

    template<>
    struct rule_for_escaped_char<symbol::esc, character<'S'>>
    {
        using type = stack<
                character<'S'>,
                symbol::make_whitespace,
                symbol::make_negated>;
    };

    template<>
    struct rule_for_escaped_char<symbol::esc, character<'l'>>
    {
        using type = stack<
                character<'l'>,
                symbol::make_lower>;
    };

    template<>
    struct rule_for_escaped_char<symbol::esc, character<'L'>>
    {
        using type = stack<
                character<'L'>,
                symbol::make_lower,
                symbol::make_negated>;
    };

    template<>
    struct rule_for_escaped_char<symbol::esc, character<'u'>>
    {
        using type = stack<
                character<'u'>,
                symbol::make_upper>;
    };

    template<>
    struct rule_for_escaped_char<symbol::esc, character<'U'>>
    {
        using type = stack<
                character<'U'>,
                symbol::make_upper,
                symbol::make_negated>;
    };

    template<>
    struct rule_for_escaped_char<symbol::esc, character<'h'>>
    {
        using type = stack<
                character<'h'>,
                symbol::make_hexa>;
    };

    template<>
    struct rule_for_escaped_char<symbol::esc, character<'H'>>
    {
        using type = stack<
                character<'H'>,
                symbol::make_hexa,
                symbol::make_negated>;
    };

    template<typename Symbol, typename Char>
    using rule_for_escaped_char_t = typename rule_for_escaped_char<Symbol, Char>::type;
}
#endif //CX_CHAR_CLASS_H
