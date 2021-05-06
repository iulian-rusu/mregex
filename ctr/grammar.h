#ifndef CTR_GRAMMAR_H
#define CTR_GRAMMAR_H

#include <type_traits>
#include "stack.h"
#include "symbol.h"

/**
 * Namespace containing the transition rules for the grammar
 */
namespace ctr::grammar
{
    // helper types defining decisions imposed by the grammar rules
    struct pop_input {};

    struct accept: std::true_type {};

    struct reject: std::false_type {};

    // defines the transition function based on two inputs
    template<typename, typename>
    struct transition
    {
        using type = reject;
    };

    // specializations of the transition function for specific inputs
    template<>
    struct transition<symbol::start, symbol::character<'('>>
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
    struct transition<symbol::start, symbol::character<')'>>
    {
        using type = reject;
    };

    template<>
    struct transition<symbol::start, symbol::character<'*'>>
    {
        using type = reject;
    };

    template<>
    struct transition<symbol::start, symbol::character<'+'>>
    {
        using type = reject;
    };

    template<>
    struct transition<symbol::start, symbol::character<'?'>>
    {
        using type = reject;
    };

    template<>
    struct transition<symbol::start, symbol::character<'\\'>>
    {
        using type = stack<
                symbol::character<'\\'>,
                symbol::esc,
                symbol::mod,
                symbol::seq,
                symbol::alt>;
    };

    template<>
    struct transition<symbol::start, symbol::character<'a'>>
    {
        using type = stack<
                symbol::character<'a'>,
                symbol::mod,
                symbol::seq,
                symbol::alt>;
    };

    template<>
    struct transition<symbol::start, symbol::character<'d'>>
    {
        using type = stack<
                symbol::character<'d'>,
                symbol::mod,
                symbol::seq,
                symbol::alt>;
    };

    template<>
    struct transition<symbol::start, symbol::character<'|'>>
    {
        using type = reject;
    };

    template<auto C>
    struct transition<symbol::start, symbol::character<C>>
    {
        using type = stack<
                symbol::character<C>,
                symbol::mod,
                symbol::seq,
                symbol::alt
        >;
    };

    template<>
    struct transition<symbol::start, symbol::epsilon>
    {
        using type = symbol::epsilon;
    };

    template<>
    struct transition<symbol::alt0, symbol::character<'('>>
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
    struct transition<symbol::alt0, symbol::character<')'>>
    {
        using type = reject;
    };

    template<>
    struct transition<symbol::alt0, symbol::character<'*'>>
    {
        using type = reject;
    };

    template<>
    struct transition<symbol::alt0, symbol::character<'+'>>
    {
        using type = reject;
    };

    template<>
    struct transition<symbol::alt0, symbol::character<'?'>>
    {
        using type = reject;
    };

    template<>
    struct transition<symbol::alt0, symbol::character<'\\'>>
    {
        using type = stack<
                symbol::character<'\\'>,
                symbol::esc,
                symbol::mod,
                symbol::seq,
                symbol::alt>;
    };

    template<>
    struct transition<symbol::alt0, symbol::character<'a'>>
    {
        using type = stack<
                symbol::character<'a'>,
                symbol::mod,
                symbol::seq,
                symbol::alt>;
    };

    template<>
    struct transition<symbol::alt0, symbol::character<'d'>>
    {
        using type = stack<
                symbol::character<'d'>,
                symbol::mod,
                symbol::seq,
                symbol::alt>;
    };

    template<>
    struct transition<symbol::alt0, symbol::character<'|'>>
    {
        using type = reject;
    };

    template<auto C>
    struct transition<symbol::alt0, symbol::character<C>>
    {
        using type = stack<
                symbol::character<C>,
                symbol::mod,
                symbol::seq,
                symbol::alt
        >;
    };

    template<>
    struct transition<symbol::alt0, symbol::epsilon>
    {
        using type = symbol::epsilon;
    };

    template<>
    struct transition<symbol::alt, symbol::character<'('>>
    {
        using type = reject;
    };

    template<>
    struct transition<symbol::alt, symbol::character<')'>>
    {
        using type = symbol::epsilon;
    };

    template<>
    struct transition<symbol::alt, symbol::character<'*'>>
    {
        using type = reject;
    };

    template<>
    struct transition<symbol::alt, symbol::character<'+'>>
    {
        using type = reject;
    };

    template<>
    struct transition<symbol::alt, symbol::character<'?'>>
    {
        using type = reject;
    };

    template<>
    struct transition<symbol::alt, symbol::character<'\\'>>
    {
        using type = reject;
    };

    template<>
    struct transition<symbol::alt, symbol::character<'a'>>
    {
        using type = reject;
    };

    template<>
    struct transition<symbol::alt, symbol::character<'d'>>
    {
        using type = reject;
    };

    template<>
    struct transition<symbol::alt, symbol::character<'|'>>
    {
        using type = stack<
                symbol::character<'|'>,
                symbol::seq0,
                symbol::alt>;
    };

    template<auto C>
    struct transition<symbol::alt, symbol::character<C>>
    {
        using type = reject;
    };

    template<>
    struct transition<symbol::alt, symbol::epsilon>
    {
        using type = symbol::epsilon;
    };

    template<>
    struct transition<symbol::esc, symbol::character<'('>>
    {
        using type = reject;
    };

    template<>
    struct transition<symbol::esc, symbol::character<')'>>
    {
        using type = symbol::epsilon;
    };

    template<>
    struct transition<symbol::esc, symbol::character<'*'>>
    {
        using type = reject;
    };

    template<>
    struct transition<symbol::esc, symbol::character<'+'>>
    {
        using type = reject;
    };

    template<>
    struct transition<symbol::esc, symbol::character<'?'>>
    {
        using type = reject;
    };

    template<>
    struct transition<symbol::esc, symbol::character<'\\'>>
    {
        using type = reject;
    };

    template<>
    struct transition<symbol::esc, symbol::character<'a'>>
    {
        using type = symbol::character<'a'>;
    };

    template<>
    struct transition<symbol::esc, symbol::character<'d'>>
    {
        using type = symbol::character<'d'>;
    };

    template<>
    struct transition<symbol::esc, symbol::character<'|'>>
    {
        using type = reject;
    };

    template<auto C>
    struct transition<symbol::esc, symbol::character<C>>
    {
        using type = reject;
    };

    template<>
    struct transition<symbol::esc, symbol::epsilon>
    {
        using type = reject;
    };

    template<>
    struct transition<symbol::mod, symbol::character<'('>>
    {
        using type = symbol::epsilon;
    };

    template<>
    struct transition<symbol::mod, symbol::character<')'>>
    {
        using type = symbol::epsilon;
    };

    template<>
    struct transition<symbol::mod, symbol::character<'*'>>
    {
        using type = symbol::character<'*'>;
    };

    template<>
    struct transition<symbol::mod, symbol::character<'+'>>
    {
        using type = symbol::character<'+'>;
    };

    template<>
    struct transition<symbol::mod, symbol::character<'?'>>
    {
        using type = symbol::character<'?'>;
    };

    template<>
    struct transition<symbol::mod, symbol::character<'\\'>>
    {
        using type = symbol::epsilon;
    };

    template<>
    struct transition<symbol::mod, symbol::character<'a'>>
    {
        using type = symbol::epsilon;
    };

    template<>
    struct transition<symbol::mod, symbol::character<'d'>>
    {
        using type = symbol::epsilon;
    };

    template<>
    struct transition<symbol::mod, symbol::character<'|'>>
    {
        using type = symbol::epsilon;
    };

    template<auto C>
    struct transition<symbol::mod, symbol::character<C>>
    {
        using type = symbol::epsilon;
    };

    template<>
    struct transition<symbol::mod, symbol::epsilon>
    {
        using type = symbol::epsilon;
    };

    template<>
    struct transition<symbol::seq0, symbol::character<'('>>
    {
        using type = stack<
                symbol::character<'('>,
                symbol::alt0,
                symbol::character<')'>,
                symbol::mod,
                symbol::seq>;
    };

    template<>
    struct transition<symbol::seq0, symbol::character<')'>>
    {
        using type = reject;
    };

    template<>
    struct transition<symbol::seq0, symbol::character<'*'>>
    {
        using type = reject;
    };

    template<>
    struct transition<symbol::seq0, symbol::character<'+'>>
    {
        using type = reject;
    };

    template<>
    struct transition<symbol::seq0, symbol::character<'?'>>
    {
        using type = reject;
    };

    template<>
    struct transition<symbol::seq0, symbol::character<'\\'>>
    {
        using type = stack<
                symbol::character<'\\'>,
                symbol::esc,
                symbol::mod,
                symbol::seq>;
    };

    template<>
    struct transition<symbol::seq0, symbol::character<'a'>>
    {
        using type = stack<
                symbol::character<'a'>,
                symbol::mod,
                symbol::seq>;
    };

    template<>
    struct transition<symbol::seq0, symbol::character<'d'>>
    {
        using type = stack<
                symbol::character<'d'>,
                symbol::mod,
                symbol::seq>;
    };

    template<>
    struct transition<symbol::seq0, symbol::character<'|'>>
    {
        using type = reject;
    };

    template<auto C>
    struct transition<symbol::seq0, symbol::character<C>>
    {
        using type = stack<
                symbol::character<C>,
                symbol::mod,
                symbol::seq>;
    };

    template<>
    struct transition<symbol::seq0, symbol::epsilon>
    {
        using type = reject;
    };

    template<>
    struct transition<symbol::seq, symbol::character<'('>>
    {
        using type = stack<
                symbol::character<'('>,
                symbol::alt0,
                symbol::character<')'>,
                symbol::mod,
                symbol::seq>;
    };

    template<>
    struct transition<symbol::seq, symbol::character<')'>>
    {
        using type = symbol::epsilon;
    };

    template<>
    struct transition<symbol::seq, symbol::character<'*'>>
    {
        using type = reject;
    };

    template<>
    struct transition<symbol::seq, symbol::character<'+'>>
    {
        using type = reject;
    };

    template<>
    struct transition<symbol::seq, symbol::character<'?'>>
    {
        using type = reject;
    };

    template<>
    struct transition<symbol::seq, symbol::character<'\\'>>
    {
        using type = stack<
                symbol::character<'\\'>,
                symbol::esc,
                symbol::mod,
                symbol::seq>;
    };

    template<>
    struct transition<symbol::seq, symbol::character<'a'>>
    {
        using type = stack<
                symbol::character<'a'>,
                symbol::mod,
                symbol::seq>;
    };

    template<>
    struct transition<symbol::seq, symbol::character<'d'>>
    {
        using type = stack<
                symbol::character<'d'>,
                symbol::mod,
                symbol::seq>;
    };

    template<>
    struct transition<symbol::seq, symbol::character<'|'>>
    {
        using type = symbol::epsilon;
    };

    template<auto C>
    struct transition<symbol::seq, symbol::character<C>>
    {
        using type = stack<
                symbol::character<C>,
                symbol::mod,
                symbol::seq>;
    };

    template<>
    struct transition<symbol::seq, symbol::epsilon>
    {
        using type = symbol::epsilon;
    };

    template<char C>
    struct transition<symbol::character<C>, symbol::character<C>>
    {
        using type = pop_input;
    };

    template<char C, typename Symbol>
    struct transition<symbol::character<C>, Symbol>
    {
        using type = reject;
    };

    template<>
    struct transition<empty_stack, symbol::epsilon>
    {
        using type = accept;
    };

    // type alias for convenience
    template<typename A, typename B>
    using transition_t = typename transition<A, B>::type;
}
#endif //CTR_GRAMMAR_H