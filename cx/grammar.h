#ifndef CX_GRAMMAR_H
#define CX_GRAMMAR_H

#include "char_class.h"

/**
 * Namespace containing the rules for the grammar
 */
namespace cx::grammar
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
    struct rule<symbol::start, character<'['>>
    {
        using type = stack<
                character<'['>,
                symbol::set_begin,
                character<']'>,
                symbol::mod,
                symbol::seq,
                symbol::alt>;
    };

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
                symbol::alt>;
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
    struct rule<symbol::start, character<'|'>>
    {
        using type = reject;
    };

    template<auto C>
    struct rule<symbol::start, character<C>>
    {
        using type = stack<
                character<C>,
                decide_action_t<C>,
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
    struct rule<symbol::alt0, character<'['>>
    {
        using type = stack<
                character<'['>,
                symbol::set_begin,
                character<']'>,
                symbol::mod,
                symbol::seq,
                symbol::alt>;
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
    struct rule<symbol::alt0, character<'|'>>
    {
        using type = reject;
    };

    template<auto C>
    struct rule<symbol::alt0, character<C>>
    {
        using type = stack<
                character<C>,
                decide_action_t<C>,
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

    template<auto C>
    struct rule<symbol::esc, character<C>>
    {
        using type = decide_rule_t<symbol::esc, character<C>>;
    };

    template<>
    struct rule<symbol::esc, symbol::epsilon>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::mod, character<'['>>
    {
        using type = symbol::epsilon;
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
    struct rule<symbol::seq0, character<'['>>
    {
        using type = stack<
                character<'['>,
                symbol::set_begin,
                character<']'>,
                symbol::mod,
                symbol::seq>;
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
    struct rule<symbol::seq0, character<'|'>>
    {
        using type = reject;
    };

    template<auto C>
    struct rule<symbol::seq0, character<C>>
    {
        using type = stack<
                character<C>,
                decide_action_t<C>,
                symbol::mod,
                symbol::seq>;
    };

    template<>
    struct rule<symbol::seq0, symbol::epsilon>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::seq, character<'['>>
    {
        using type = stack<
                character<'['>,
                symbol::set_begin,
                character<']'>,
                symbol::mod,
                symbol::make_sequence,
                symbol::seq>;
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
    struct rule<symbol::seq, character<'|'>>
    {
        using type = symbol::epsilon;
    };

    template<auto C>
    struct rule<symbol::seq, character<C>>
    {
        using type = stack<
                character<C>,
                decide_action_t<C>,
                symbol::mod,
                symbol::make_sequence,
                symbol::seq>;
    };

    template<>
    struct rule<symbol::seq, symbol::epsilon>
    {
        using type = symbol::epsilon;
    };

    // set-specific rules
    template<>
    struct rule<symbol::set_begin, character<'^'>>
    {
        using type = stack<
                character<'^'>,
                symbol::set_seq,
                symbol::make_negated>;
    };

    template<>
    struct rule<symbol::set_begin, character<'\\'>>
    {
        using type = stack<
                character<'\\'>,
                symbol::set_esc,
                symbol::make_set_from_stack,
                symbol::set_seq>;
    };

    template<auto C>
    struct rule<symbol::set_begin, character<C>>
    {
        using type = stack<
                character<C>,
                symbol::make_set_from_current_char,
                symbol::set_seq>;
    };

    template<>
    struct rule<symbol::set_seq, character<']'>>
    {
        using type = symbol::epsilon;
    };

    template<>
    struct rule<symbol::set_seq, character<'\\'>>
    {
        using type = stack<
                character<'\\'>,
                symbol::set_esc,
                symbol::make_set_from_stack,
                symbol::set_seq>;
    };

    template<>
    struct rule<symbol::set_seq, character<'-'>>
    {
        using type = stack<
                character<'-'>,
                symbol::set_range_start,
                symbol::set_seq>;
    };

    template<auto C>
    struct rule<symbol::set_seq, character<C>>
    {
        using type = stack<
                character<C>,
                symbol::make_set_from_current_char,
                symbol::set_seq>;
    };

    template<auto C>
    struct rule<symbol::set_range_start, character<C>>
    {
        using type = stack<
                character<C>,
                symbol::make_range,
                symbol::set_seq0>;
    };

    template<>
    struct rule<symbol::set_range_start, character<']'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::set_seq0, character<'\\'>>
    {
        using type = stack<
                character<'\\'>,
                symbol::set_esc,
                symbol::make_set_from_stack,
                symbol::set_seq>;
    };

    template<>
    struct rule<symbol::set_seq0, character<']'>>
    {
        using type = symbol::epsilon;
    };

    template<>
    struct rule<symbol::set_seq0, character<'-'>>
    {
        using type = reject;
    };

    template<auto C>
    struct rule<symbol::set_seq0, character<C>>
    {
        using type = stack<
                character<C>,
                symbol::make_set_from_current_char,
                symbol::set_seq>;
    };

    template<auto C>
    struct rule<symbol::set_esc, character<C>>
    {
        using type = decide_rule_t<symbol::set_esc, character<C>>;
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
#endif //CX_GRAMMAR_H