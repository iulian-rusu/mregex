#ifndef CTR_GRAMMAR_H
#define CTR_GRAMMAR_H

#include <type_traits>
#include "stack.h"
#include "symbol.h"

/**
 * Namespace containing the rule rules for the grammar
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
    struct rule<symbol::start, symbol::character<'('>>
    {
        using type = stack<
                symbol::character<'('>,
                symbol::alt0,
                symbol::character<')'>,
                symbol::mod,
                symbol::seq,
                symbol::alt>; // this rule pushes a set of symbols on the stack
    };

    template<>
    struct rule<symbol::start, symbol::character<')'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::start, symbol::character<'*'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::start, symbol::character<'+'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::start, symbol::character<'?'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::start, symbol::character<'\\'>>
    {
        using type = stack<
                symbol::character<'\\'>,
                symbol::esc,
                symbol::mod,
                symbol::seq,
                symbol::alt>;
    };

    template<>
    struct rule<symbol::start, symbol::character<'a'>>
    {
        using type = stack<
                symbol::character<'a'>,
                symbol::mod,
                symbol::seq,
                symbol::alt>;
    };

    template<>
    struct rule<symbol::start, symbol::character<'d'>>
    {
        using type = stack<
                symbol::character<'d'>,
                symbol::mod,
                symbol::seq,
                symbol::alt>;
    };

    template<>
    struct rule<symbol::start, symbol::character<'|'>>
    {
        using type = reject;
    };

    template<auto C>
    struct rule<symbol::start, symbol::character<C>>
    {
        using type = stack<
                symbol::character<C>,
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
    struct rule<symbol::alt0, symbol::character<'('>>
    {
        using type = stack<
                symbol::character<'('>,
                symbol::alt0,
                symbol::character<')'>,
                symbol::mod,
                symbol::seq,
                symbol::alt>;
    };

    template<>
    struct rule<symbol::alt0, symbol::character<')'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::alt0, symbol::character<'*'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::alt0, symbol::character<'+'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::alt0, symbol::character<'?'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::alt0, symbol::character<'\\'>>
    {
        using type = stack<
                symbol::character<'\\'>,
                symbol::esc,
                symbol::mod,
                symbol::seq,
                symbol::alt>;
    };

    template<>
    struct rule<symbol::alt0, symbol::character<'a'>>
    {
        using type = stack<
                symbol::character<'a'>,
                symbol::mod,
                symbol::seq,
                symbol::alt>;
    };

    template<>
    struct rule<symbol::alt0, symbol::character<'d'>>
    {
        using type = stack<
                symbol::character<'d'>,
                symbol::mod,
                symbol::seq,
                symbol::alt>;
    };

    template<>
    struct rule<symbol::alt0, symbol::character<'|'>>
    {
        using type = reject;
    };

    template<auto C>
    struct rule<symbol::alt0, symbol::character<C>>
    {
        using type = stack<
                symbol::character<C>,
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
    struct rule<symbol::alt, symbol::character<'('>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::alt, symbol::character<')'>>
    {
        using type = symbol::epsilon;
    };

    template<>
    struct rule<symbol::alt, symbol::character<'*'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::alt, symbol::character<'+'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::alt, symbol::character<'?'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::alt, symbol::character<'\\'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::alt, symbol::character<'a'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::alt, symbol::character<'d'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::alt, symbol::character<'|'>>
    {
        using type = stack<
                symbol::character<'|'>,
                symbol::seq0,
                symbol::alt>;
    };

    template<auto C>
    struct rule<symbol::alt, symbol::character<C>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::alt, symbol::epsilon>
    {
        using type = symbol::epsilon;
    };

    template<>
    struct rule<symbol::esc, symbol::character<'('>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::esc, symbol::character<')'>>
    {
        using type = symbol::epsilon;
    };

    template<>
    struct rule<symbol::esc, symbol::character<'*'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::esc, symbol::character<'+'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::esc, symbol::character<'?'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::esc, symbol::character<'\\'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::esc, symbol::character<'a'>>
    {
        using type = symbol::character<'a'>;
    };

    template<>
    struct rule<symbol::esc, symbol::character<'d'>>
    {
        using type = symbol::character<'d'>;
    };

    template<>
    struct rule<symbol::esc, symbol::character<'|'>>
    {
        using type = reject;
    };

    template<auto C>
    struct rule<symbol::esc, symbol::character<C>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::esc, symbol::epsilon>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::mod, symbol::character<'('>>
    {
        using type = symbol::epsilon;
    };

    template<>
    struct rule<symbol::mod, symbol::character<')'>>
    {
        using type = symbol::epsilon;
    };

    template<>
    struct rule<symbol::mod, symbol::character<'*'>>
    {
        using type = symbol::character<'*'>;
    };

    template<>
    struct rule<symbol::mod, symbol::character<'+'>>
    {
        using type = symbol::character<'+'>;
    };

    template<>
    struct rule<symbol::mod, symbol::character<'?'>>
    {
        using type = symbol::character<'?'>;
    };

    template<>
    struct rule<symbol::mod, symbol::character<'\\'>>
    {
        using type = symbol::epsilon;
    };

    template<>
    struct rule<symbol::mod, symbol::character<'a'>>
    {
        using type = symbol::epsilon;
    };

    template<>
    struct rule<symbol::mod, symbol::character<'d'>>
    {
        using type = symbol::epsilon;
    };

    template<>
    struct rule<symbol::mod, symbol::character<'|'>>
    {
        using type = symbol::epsilon;
    };

    template<auto C>
    struct rule<symbol::mod, symbol::character<C>>
    {
        using type = symbol::epsilon;
    };

    template<>
    struct rule<symbol::mod, symbol::epsilon>
    {
        using type = symbol::epsilon;
    };

    template<>
    struct rule<symbol::seq0, symbol::character<'('>>
    {
        using type = stack<
                symbol::character<'('>,
                symbol::alt0,
                symbol::character<')'>,
                symbol::mod,
                symbol::seq>;
    };

    template<>
    struct rule<symbol::seq0, symbol::character<')'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::seq0, symbol::character<'*'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::seq0, symbol::character<'+'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::seq0, symbol::character<'?'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::seq0, symbol::character<'\\'>>
    {
        using type = stack<
                symbol::character<'\\'>,
                symbol::esc,
                symbol::mod,
                symbol::seq>;
    };

    template<>
    struct rule<symbol::seq0, symbol::character<'a'>>
    {
        using type = stack<
                symbol::character<'a'>,
                symbol::mod,
                symbol::seq>;
    };

    template<>
    struct rule<symbol::seq0, symbol::character<'d'>>
    {
        using type = stack<
                symbol::character<'d'>,
                symbol::mod,
                symbol::seq>;
    };

    template<>
    struct rule<symbol::seq0, symbol::character<'|'>>
    {
        using type = reject;
    };

    template<auto C>
    struct rule<symbol::seq0, symbol::character<C>>
    {
        using type = stack<
                symbol::character<C>,
                symbol::mod,
                symbol::seq>;
    };

    template<>
    struct rule<symbol::seq0, symbol::epsilon>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::seq, symbol::character<'('>>
    {
        using type = stack<
                symbol::character<'('>,
                symbol::alt0,
                symbol::character<')'>,
                symbol::mod,
                symbol::seq>;
    };

    template<>
    struct rule<symbol::seq, symbol::character<')'>>
    {
        using type = symbol::epsilon;
    };

    template<>
    struct rule<symbol::seq, symbol::character<'*'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::seq, symbol::character<'+'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::seq, symbol::character<'?'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::seq, symbol::character<'\\'>>
    {
        using type = stack<
                symbol::character<'\\'>,
                symbol::esc,
                symbol::mod,
                symbol::seq>;
    };

    template<>
    struct rule<symbol::seq, symbol::character<'a'>>
    {
        using type = stack<
                symbol::character<'a'>,
                symbol::mod,
                symbol::seq>;
    };

    template<>
    struct rule<symbol::seq, symbol::character<'d'>>
    {
        using type = stack<
                symbol::character<'d'>,
                symbol::mod,
                symbol::seq>;
    };

    template<>
    struct rule<symbol::seq, symbol::character<'|'>>
    {
        using type = symbol::epsilon;
    };

    template<auto C>
    struct rule<symbol::seq, symbol::character<C>>
    {
        using type = stack<
                symbol::character<C>,
                symbol::mod,
                symbol::seq>;
    };

    template<>
    struct rule<symbol::seq, symbol::epsilon>
    {
        using type = symbol::epsilon;
    };

    template<char C>
    struct rule<symbol::character<C>, symbol::character<C>>
    {
        using type = pop_input;
    };

    template<char C, typename Symbol>
    struct rule<symbol::character<C>, Symbol>
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