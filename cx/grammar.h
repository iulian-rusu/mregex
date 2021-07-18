#ifndef CX_GRAMMAR_H
#define CX_GRAMMAR_H

#include "char_class.h"
#include "backref_builder.h"
#include "quantifier_builder.h"

namespace cx::grammar
{
    // Helper types defining decisions imposed by the grammar rules
    struct advance_input {};

    struct accept: std::true_type {};

    struct reject: std::false_type {};

    /**
     * Metafunction defining a grammar rule based on two parameters.
     *
     * @tparam  The current symbol on the stack
     * @tparam  The current character in the input string
     */
    template<typename, typename>
    struct rule
    {
        using type = reject;
    };

    // Specialized rules for specific inputs
    template<>
    struct rule<symbol::start, character<'['>>
    {
        using type =
                stack
                <
                    character<'['>,
                    symbol::make_set,
                    symbol::set_begin,
                    character<']'>,
                    symbol::mod,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::start, character<'('>>
    {
        using type =
                stack
                <
                    character<'('>,
                    symbol::capture_begin,
                    character<')'>,
                    symbol::make_capturing,
                    symbol::mod,
                    symbol::seq,
                    symbol::alt
                >;
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
        using type =
                stack
                <
                    character<'\\'>,
                    symbol::esc,
                    symbol::mod,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::start, character<'|'>>
    {
        using type = reject;
    };

    template<auto C>
    struct rule<symbol::start, character<C>>
    {
        using type =
                stack
                <
                    character<C>,
                    symbol::make_char,
                    symbol::mod,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::start, character<'.'>>
    {
        using type =
                stack
                <
                    character<'.'>,
                    symbol::make_wildcard,
                    symbol::mod,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::start, character<'^'>>
    {
        using type =
                stack
                <
                    character<'^'>,
                    symbol::make_beginning,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::start, character<'$'>>
    {
        using type =
                stack
                <
                    character<'$'>,
                    symbol::make_ending,
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
    struct rule<symbol::capture_begin, character<'['>>
    {
        using type =
                stack
                <
                    character<'['>,
                    symbol::make_set,
                    symbol::set_begin,
                    character<']'>,
                    symbol::mod,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::capture_begin, character<'('>>
    {
        using type =
                stack
                <
                    character<'('>,
                    symbol::capture_begin,
                    character<')'>,
                    symbol::make_capturing,
                    symbol::mod,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::capture_begin, character<')'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::capture_begin, character<'*'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::capture_begin, character<'+'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::capture_begin, character<'\\'>>
    {
        using type =
                stack
                <
                    character<'\\'>,
                    symbol::esc,
                    symbol::mod,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::capture_begin, character<'|'>>
    {
        using type = reject;
    };

    template<auto C>
    struct rule<symbol::capture_begin, character<C>>
    {
        using type =
                stack
                <
                    character<C>,
                    symbol::make_char,
                    symbol::mod,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::capture_begin, character<'.'>>
    {
        using type =
                stack
                <
                    character<'.'>,
                    symbol::make_wildcard,
                    symbol::mod,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::capture_begin, character<'^'>>
    {
        using type =
                stack
                <
                    character<'^'>,
                    symbol::make_beginning,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::capture_begin, symbol::epsilon>
    {
        using type = symbol::epsilon;
    };

    template<>
    struct rule<symbol::capture_begin, character<'?'>>
    {
        using type =
                stack
                <
                    character<'?'>,
                    symbol::capture_mod
                >;
    };

    template<>
    struct rule<symbol::capture_mod, character<':'>>
    {
        using type =
                stack
                <
                    character<':'>,
                    symbol::capture_begin_no_mod,
                    symbol::make_captureless
                >;
    };

    template<>
    struct rule<symbol::capture_mod, character<'>'>>
    {
        using type =
                stack
                <
                    character<'>'>,
                    symbol::capture_begin_no_mod,
                    symbol::make_atomic
                >;
    };

    template<auto C>
    struct rule<symbol::capture_mod, character<C>>
    {
        using type = reject;
    };

    // Special begin marker symbol for captureless subexpressions
    template<>
    struct rule<symbol::capture_begin_no_mod, character<'['>>
    {
        using type =
                stack
                <
                    character<'['>,
                    symbol::make_set,
                    symbol::set_begin,
                    character<']'>,
                    symbol::mod,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::capture_begin_no_mod, character<'('>>
    {
        using type =
                stack
                <
                    character<'('>,
                    symbol::capture_begin,
                    character<')'>,
                    symbol::make_capturing,
                    symbol::mod,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::capture_begin_no_mod, character<')'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::capture_begin_no_mod, character<'?'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::capture_begin_no_mod, character<'*'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::capture_begin_no_mod, character<'+'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::capture_begin_no_mod, character<'\\'>>
    {
        using type =
                stack
                <
                    character<'\\'>,
                    symbol::esc,
                    symbol::mod,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::capture_begin_no_mod, character<'|'>>
    {
        using type = reject;
    };

    template<auto C>
    struct rule<symbol::capture_begin_no_mod, character<C>>
    {
        using type =
                stack
                <
                    character<C>,
                    symbol::make_char,
                    symbol::mod,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::capture_begin_no_mod, character<'.'>>
    {
        using type =
                stack
                <
                    character<'.'>,
                    symbol::make_wildcard,
                    symbol::mod,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::capture_begin_no_mod, character<'^'>>
    {
        using type =
                stack
                <
                    character<'^'>,
                    symbol::make_beginning,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::capture_begin_no_mod, symbol::epsilon>
    {
        using type = symbol::epsilon;
    };

    template<>
    struct rule<symbol::alt, character<')'>>
    {
        using type = symbol::epsilon;
    };

    template<>
    struct rule<symbol::alt, character<'|'>>
    {
        using type =
                stack
                <
                    character<'|'>,
                    symbol::seq0,
                    symbol::make_alternation,
                    symbol::alt
                >;
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
        using type = decide_esc_rule_t<C>;
    };

    template<>
    struct rule<symbol::esc, symbol::epsilon>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::mod, character<'*'>>
    {
        using type =
                stack
                <
                    character<'*'>,
                    symbol::make_star
                >;
    };

    template<>
    struct rule<symbol::mod, character<'+'>>
    {
        using type =
                stack
                <
                    character<'+'>,
                    symbol::make_plus
                >;
    };

    template<>
    struct rule<symbol::mod, character<'?'>>
    {
        using type =
                stack
                <
                    character<'?'>,
                    symbol::make_optional
                >;
    };

    template<>
    struct rule<symbol::mod, character<'{'>>
    {
        using type =
                stack
                <
                    character<'{'>,
                    symbol::quantifier_begin
                >;
    };

    template<auto C>
    struct rule<symbol::quantifier_begin, character<C>>
    {
        using type = begin_quantifier_value_t<C>;
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

    template<std::size_t N, auto C>
    struct rule<symbol::quantifier_value<N>, character<C>>
    {
        using type =
                std::conditional_t
                <
                    is_numeric_v<C>,
                    stack
                    <
                        character<C>,
                        symbol::quantifier_value<10 * N + C - '0'>
                    >,
                    reject
                >;
    };

    template<std::size_t N>
    struct rule<symbol::quantifier_value<N>, character<'}'>>
    {
        using type =
                stack
                <
                    character<'}'>,
                    symbol::make_repeated<N>
                >;
    };

    template<>
    struct rule<symbol::seq0, character<'['>>
    {
        using type =
                stack
                <
                    character<'['>,
                    symbol::make_set,
                    symbol::set_begin,
                    character<']'>,
                    symbol::mod,
                    symbol::seq
                >;
    };

    template<>
    struct rule<symbol::seq0, character<'('>>
    {
        using type =
                stack
                <
                    character<'('>,
                    symbol::capture_begin,
                    character<')'>,
                    symbol::make_capturing,
                    symbol::mod,
                    symbol::seq
                >;
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
        using type =
                stack
                <
                    character<'\\'>,
                    symbol::esc,
                    symbol::mod,
                    symbol::seq
                >;
    };

    template<>
    struct rule<symbol::seq0, character<'|'>>
    {
        using type = reject;
    };

    template<auto C>
    struct rule<symbol::seq0, character<C>>
    {
        using type =
                stack
                <
                    character<C>,
                    symbol::make_char,
                    symbol::mod,
                    symbol::seq
                >;
    };

    template<>
    struct rule<symbol::seq0, character<'.'>>
    {
        using type =
                stack
                <
                    character<'.'>,
                    symbol::make_wildcard,
                    symbol::mod,
                    symbol::seq
                >;
    };

    template<>
    struct rule<symbol::seq0, character<'^'>>
    {
        using type =
                stack
                <
                    character<'^'>,
                    symbol::make_beginning,
                    symbol::seq
                >;
    };

    template<>
    struct rule<symbol::seq0, character<'$'>>
    {
        using type =
                stack
                <
                    character<'$'>,
                    symbol::make_ending,
                    symbol::seq
                >;
    };

    template<>
    struct rule<symbol::seq0, symbol::epsilon>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::seq, character<'['>>
    {
        using type =
                stack
                <
                    character<'['>,
                    symbol::make_set,
                    symbol::set_begin,
                    character<']'>,
                    symbol::mod,
                    symbol::make_sequence,
                    symbol::seq
                >;
    };

    template<>
    struct rule<symbol::seq, character<'('>>
    {
        using type =
                stack
                <
                    character<'('>,
                    symbol::capture_begin,
                    character<')'>,
                    symbol::make_capturing,
                    symbol::mod,
                    symbol::make_sequence,
                    symbol::seq
                >;
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
        using type =
                stack
                <
                    character<'\\'>,
                    symbol::esc,
                    symbol::mod,
                    symbol::make_sequence,
                    symbol::seq
                >;
    };

    template<>
    struct rule<symbol::seq, character<'|'>>
    {
        using type = symbol::epsilon;
    };

    template<auto C>
    struct rule<symbol::seq, character<C>>
    {
        using type =
                stack
                <
                    character<C>,
                    symbol::make_char,
                    symbol::mod,
                    symbol::make_sequence,
                    symbol::seq
                >;
    };

    template<>
    struct rule<symbol::seq, character<'.'>>
    {
        using type =
                stack
                <
                    character<'.'>,
                    symbol::make_wildcard,
                    symbol::mod,
                    symbol::make_sequence,
                    symbol::seq
                >;
    };

    template<>
    struct rule<symbol::seq, character<'^'>>
    {
        using type =
                stack
                <
                    character<'^'>,
                    symbol::make_beginning,
                    symbol::make_sequence,
                    symbol::seq
                >;
    };

    template<>
    struct rule<symbol::seq, character<'$'>>
    {
        using type =
                stack
                <
                    character<'$'>,
                    symbol::make_ending,
                    symbol::make_sequence,
                    symbol::seq
                >;
    };

    template<>
    struct rule<symbol::seq, symbol::epsilon>
    {
        using type = symbol::epsilon;
    };

    // Rules for parsing sets
    template<>
    struct rule<symbol::set_begin, character<'^'>>
    {
        using type =
                stack
                <
                    character<'^'>,
                    symbol::set_begin_no_neg,
                    symbol::make_negated
                >;
    };

    template<>
    struct rule<symbol::set_begin, character<'\\'>>
    {
        using type =
                stack
                <
                    character<'\\'>,
                    symbol::set_esc,
                    symbol::make_set_from_stack,
                    symbol::set_seq
                >;
    };

    template<>
    struct rule<symbol::set_begin, character<']'>>
    {
        using type = symbol::epsilon;
    };

    template<auto C>
    struct rule<symbol::set_begin, character<C>>
    {
        using type =
                stack
                <
                    character<C>,
                    symbol::make_set_from_current_char,
                    symbol::set_seq
                >;
    };

    template<>
    struct rule<symbol::set_begin_no_neg, character<'\\'>>
    {
        using type =
                stack
                <
                    character<'\\'>,
                    symbol::set_esc,
                    symbol::make_set_from_stack,
                    symbol::set_seq
                >;
    };

    template<auto C>
    struct rule<symbol::set_begin_no_neg, character<C>>
    {
        using type =
                stack
                <
                    character<C>,
                    symbol::make_set_from_current_char,
                    symbol::set_seq
                >;
    };

    template<>
    struct rule<symbol::set_seq, character<']'>>
    {
        using type = symbol::epsilon;
    };

    template<>
    struct rule<symbol::set_seq, character<'\\'>>
    {
        using type =
                stack
                <
                    character<'\\'>,
                    symbol::set_esc,
                    symbol::make_set_from_stack,
                    symbol::set_seq
                >;
    };

    template<>
    struct rule<symbol::set_seq, character<'-'>>
    {
        using type =
                stack
                <
                    character<'-'>,
                    symbol::set_range_start,
                    symbol::set_seq
                >;
    };

    template<auto C>
    struct rule<symbol::set_seq, character<C>>
    {
        using type =
                stack
                <
                    character<C>,
                    symbol::make_set_from_current_char,
                    symbol::set_seq
                >;
    };

    template<auto C>
    struct rule<symbol::set_range_start, character<C>>
    {
        using type =
                stack
                <
                    character<C>,
                    symbol::make_range,
                    symbol::set_seq0
                >;
    };

    template<>
    struct rule<symbol::set_range_start, character<']'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::set_range_start, character<'\\'>>
    {
        using type =
                stack
                <
                    character<'\\'>,
                    symbol::set_range_esc,
                    symbol::make_set_from_stack,
                    symbol::make_range_from_stack,
                    symbol::set_seq0
                >;
    };

    template<>
    struct rule<symbol::set_seq0, character<'\\'>>
    {
        using type =
                stack
                <
                    character<'\\'>,
                    symbol::set_esc,
                    symbol::make_set_from_stack,
                    symbol::set_seq
                >;
    };

    template<>
    struct rule<symbol::set_seq0, character<']'>>
    {
        using type = symbol::epsilon;
    };

    template<auto C>
    struct rule<symbol::set_seq0, character<C>>
    {
        using type =
                stack
                <
                    character<C>,
                    symbol::make_set_from_current_char,
                    symbol::set_seq
                >;
    };

    template<auto C>
    struct rule<symbol::set_esc, character<C>>
    {
        using type = rule_for_char_class_t<C>;
    };

    template<auto C>
    struct rule<symbol::set_range_esc, character<C>>
    {
        using type =
                stack
                <
                    character<C>,
                    symbol::make_char
                >;
    };

    template<typename Char>
    struct rule<symbol::set_range_esc, Char>
    {
        using type = reject;
    };

    // Rules for parsing backreferences
    template<std::size_t ID, auto C>
    struct rule<symbol::backref_id<ID>, character<C>>
    {
        using type = continue_backref_id_t<C, ID>;
    };

    template<std::size_t ID>
    struct rule<symbol::backref_id<ID>, symbol::epsilon>
    {
        using type = symbol::make_backref<ID>;
    };

    template<char C>
    struct rule<character<C>, character<C>>
    {
        using type = advance_input;
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

    template<typename A, typename B>
    using rule_t = typename rule<A, B>::type;
}
#endif //CX_GRAMMAR_H