#ifndef CTR_GRAMMAR_H
#define CTR_GRAMMAR_H

#include <type_traits>
#include "stack.h"
#include "symbol.h"
#include "ast.h"

/**
 * Namespace containing the rules for the grammar
 */
namespace ctr::grammar
{
    // helper types defining decisions imposed by the grammar rules
    struct pop_input {};

    struct accept: std::true_type {};

    struct reject: std::false_type {};

    // defines a rule based on two inputs
    // rules are transcribed from a parsing table based on regular expression grammar
    template<typename, typename>
    struct rule
    {
        using type = reject; // the default rule is to reject the input
    };

    // specialized rules for specific inputs
    template<>
    struct rule<symbol::start, character<'('>>
    {
        using type = stack<
                character<'('>,
                symbol::alt0,
                character<')'>,
                symbol::make_capturing,
                symbol::mod,
                symbol::seq,
                symbol::alt>; // this rule pushes a set of symbols on the stack
    };

    template<>
    struct rule<symbol::start, character<')'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::start, character<'*'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::start, character<'+'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::start, character<'?'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::start, character<'\\'>>
    {
        using type = stack<
                character<'\\'>,
                symbol::esc,
                symbol::mod,
                symbol::seq,
                symbol::alt>;
    };

    template<>
    struct rule<symbol::start, character<'a'>>
    {
        using type = stack<
                character<'a'>,
                symbol::make_char,
                symbol::mod,
                symbol::seq,
                symbol::alt>;
    };

    template<>
    struct rule<symbol::start, character<'d'>>
    {
        using type = stack<
                character<'d'>,
                symbol::make_char,
                symbol::mod,
                symbol::seq,
                symbol::alt>;
    };

    template<>
    struct rule<symbol::start, character<'|'>>
    {
        using type = reject;
    };

    template<auto C>
    struct rule<symbol::start, character<C>>
    {
        using type = stack<
                character<C>,
                symbol::make_char,
                symbol::mod,
                symbol::seq,
                symbol::alt
        >;
    };

    template<>
    struct rule<symbol::start, symbol::epsilon>
    {
        using type = symbol::epsilon;
    };

    template<>
    struct rule<symbol::alt0, character<'('>>
    {
        using type = stack<
                character<'('>,
                symbol::alt0,
                character<')'>,
                symbol::make_capturing,
                symbol::mod,
                symbol::seq,
                symbol::alt>;
    };

    template<>
    struct rule<symbol::alt0, character<')'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::alt0, character<'*'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::alt0, character<'+'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::alt0, character<'?'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::alt0, character<'\\'>>
    {
        using type = stack<
                character<'\\'>,
                symbol::esc,
                symbol::mod,
                symbol::seq,
                symbol::alt>;
    };

    template<>
    struct rule<symbol::alt0, character<'a'>>
    {
        using type = stack<
                character<'a'>,
                symbol::make_char,
                symbol::mod,
                symbol::seq,
                symbol::alt>;
    };

    template<>
    struct rule<symbol::alt0, character<'d'>>
    {
        using type = stack<
                character<'d'>,
                symbol::make_char,
                symbol::mod,
                symbol::seq,
                symbol::alt>;
    };

    template<>
    struct rule<symbol::alt0, character<'|'>>
    {
        using type = reject;
    };

    template<auto C>
    struct rule<symbol::alt0, character<C>>
    {
        using type = stack<
                character<C>,
                symbol::make_char,
                symbol::mod,
                symbol::seq,
                symbol::alt
        >;
    };

    template<>
    struct rule<symbol::alt0, symbol::epsilon>
    {
        using type = symbol::epsilon;
    };

    template<>
    struct rule<symbol::alt, character<'('>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::alt, character<')'>>
    {
        using type = symbol::epsilon;
    };

    template<>
    struct rule<symbol::alt, character<'*'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::alt, character<'+'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::alt, character<'?'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::alt, character<'\\'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::alt, character<'a'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::alt, character<'d'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::alt, character<'|'>>
    {
        using type = stack<
                character<'|'>,
                symbol::seq0,
                symbol::make_alternation,
                symbol::alt>;
    };

    template<auto C>
    struct rule<symbol::alt, character<C>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::alt, symbol::epsilon>
    {
        using type = symbol::epsilon;
    };

    template<>
    struct rule<symbol::esc, character<'a'>>
    {
        using type = stack<
                character<'a'>,
                symbol::make_alnum>;
    };

    template<>
    struct rule<symbol::esc, character<'A'>>
    {
        using type = stack<
                character<'A'>,
                symbol::make_alnum,
                symbol::make_negated>;
    };

    template<>
    struct rule<symbol::esc, character<'d'>>
    {
        using type = stack<
                character<'d'>,
                symbol::make_digit>;
    };

    template<>
    struct rule<symbol::esc, character<'D'>>
    {
        using type = stack<
                character<'D'>,
                symbol::make_digit,
                symbol::make_negated>;
    };
    template<>
    struct rule<symbol::esc, character<'w'>>
    {
        using type = stack<
                character<'w'>,
                symbol::make_word>;
    };

    template<>
    struct rule<symbol::esc, character<'W'>>
    {
        using type = stack<
                character<'W'>,
                symbol::make_word,
                symbol::make_negated>;
    };

    template<>
    struct rule<symbol::esc, character<'s'>>
    {
        using type = stack<
                character<'s'>,
                symbol::make_whitespace>;
    };

    template<>
    struct rule<symbol::esc, character<'S'>>
    {
        using type = stack<
                character<'S'>,
                symbol::make_whitespace,
                symbol::make_negated>;
    };

    template<>
    struct rule<symbol::esc, character<'l'>>
    {
        using type = stack<
                character<'l'>,
                symbol::make_lower>;
    };

    template<>
    struct rule<symbol::esc, character<'L'>>
    {
        using type = stack<
                character<'L'>,
                symbol::make_lower,
                symbol::make_negated>;
    };

    template<>
    struct rule<symbol::esc, character<'u'>>
    {
        using type = stack<
                character<'u'>,
                symbol::make_upper>;
    };

    template<>
    struct rule<symbol::esc, character<'U'>>
    {
        using type = stack<
                character<'U'>,
                symbol::make_upper,
                symbol::make_negated>;
    };

    template<>
    struct rule<symbol::esc, character<'h'>>
    {
        using type = stack<
                character<'h'>,
                symbol::make_hexa>;
    };

    template<>
    struct rule<symbol::esc, character<'H'>>
    {
        using type = stack<
                character<'H'>,
                symbol::make_hexa,
                symbol::make_negated>;
    };

    template<auto C>
    struct rule<symbol::esc, character<C>>
    {
        using type = stack<
                character<C>,
                symbol::make_char>;
    };

    template<>
    struct rule<symbol::esc, symbol::epsilon>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::mod, character<'('>>
    {
        using type = symbol::epsilon;
    };

    template<>
    struct rule<symbol::mod, character<')'>>
    {
        using type = symbol::epsilon;
    };

    template<>
    struct rule<symbol::mod, character<'*'>>
    {
        using type = stack<
                character<'*'>,
                symbol::make_star>;
    };

    template<>
    struct rule<symbol::mod, character<'+'>>
    {
        using type = stack<
                character<'+'>,
                symbol::make_plus>;
    };

    template<>
    struct rule<symbol::mod, character<'?'>>
    {
        using type = stack<
                character<'?'>,
                symbol::make_optional>;
    };

    template<>
    struct rule<symbol::mod, character<'\\'>>
    {
        using type = symbol::epsilon;
    };

    template<>
    struct rule<symbol::mod, character<'a'>>
    {
        using type = symbol::epsilon;
    };

    template<>
    struct rule<symbol::mod, character<'d'>>
    {
        using type = symbol::epsilon;
    };

    template<>
    struct rule<symbol::mod, character<'|'>>
    {
        using type = symbol::epsilon;
    };

    template<auto C>
    struct rule<symbol::mod, character<C>>
    {
        using type = symbol::epsilon;
    };

    template<>
    struct rule<symbol::mod, symbol::epsilon>
    {
        using type = symbol::epsilon;
    };

    template<>
    struct rule<symbol::seq0, character<'('>>
    {
        using type = stack<
                character<'('>,
                symbol::alt0,
                character<')'>,
                symbol::make_capturing,
                symbol::mod,
                symbol::seq>;
    };

    template<>
    struct rule<symbol::seq0, character<')'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::seq0, character<'*'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::seq0, character<'+'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::seq0, character<'?'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::seq0, character<'\\'>>
    {
        using type = stack<
                character<'\\'>,
                symbol::esc,
                symbol::mod,
                symbol::seq>;
    };

    template<>
    struct rule<symbol::seq0, character<'a'>>
    {
        using type = stack<
                character<'a'>,
                symbol::make_char,
                symbol::mod,
                symbol::seq>;
    };

    template<>
    struct rule<symbol::seq0, character<'d'>>
    {
        using type = stack<
                character<'d'>,
                symbol::make_char,
                symbol::mod,
                symbol::seq>;
    };

    template<>
    struct rule<symbol::seq0, character<'|'>>
    {
        using type = reject;
    };

    template<auto C>
    struct rule<symbol::seq0, character<C>>
    {
        using type = stack<
                character<C>,
                symbol::make_char,
                symbol::mod,
                symbol::seq>;
    };

    template<>
    struct rule<symbol::seq0, symbol::epsilon>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::seq, character<'('>>
    {
        using type = stack<
                character<'('>,
                symbol::alt0,
                character<')'>,
                symbol::make_capturing,
                symbol::mod,
                symbol::make_sequence,
                symbol::seq>;
    };

    template<>
    struct rule<symbol::seq, character<')'>>
    {
        using type = symbol::epsilon;
    };

    template<>
    struct rule<symbol::seq, character<'*'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::seq, character<'+'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::seq, character<'?'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::seq, character<'\\'>>
    {
        using type = stack<
                character<'\\'>,
                symbol::esc,
                symbol::mod,
                symbol::make_sequence,
                symbol::seq>;
    };

    template<>
    struct rule<symbol::seq, character<'a'>>
    {
        using type = stack<
                character<'a'>,
                symbol::make_char,
                symbol::mod,
                symbol::make_sequence,
                symbol::seq>;
    };

    template<>
    struct rule<symbol::seq, character<'d'>>
    {
        using type = stack<
                character<'d'>,
                symbol::make_char,
                symbol::mod,
                symbol::make_sequence,
                symbol::seq>;
    };

    template<>
    struct rule<symbol::seq, character<'|'>>
    {
        using type = symbol::epsilon;
    };

    template<auto C>
    struct rule<symbol::seq, character<C>>
    {
        using type = stack<
                character<C>,
                symbol::make_char,
                symbol::mod,
                symbol::make_sequence,
                symbol::seq>;
    };

    template<>
    struct rule<symbol::seq, symbol::epsilon>
    {
        using type = symbol::epsilon;
    };

    template<char C>
    struct rule<character<C>, character<C>>
    {
        using type = pop_input;
    };

    template<char C, typename Symbol>
    struct rule<character<C>, Symbol>
    {
        using type = reject;
    };

    template<>
    struct rule<empty_stack, symbol::epsilon>
    {
        using type = accept;
    };

    // type alias for easier usage
    template<typename A, typename B>
    using rule_t = typename rule<A, B>::type;
}
#endif //CTR_GRAMMAR_H