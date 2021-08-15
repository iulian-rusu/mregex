#ifndef META_GRAMMAR_HPP
#define META_GRAMMAR_HPP

#include "esc_rules.hpp"
#include "quantifier_rules.hpp"

namespace meta::grammar
{
    /**
     * Metafunction defining a grammar rule based on two parameters.
     *
     * @tparam S  The current symbol on the stack
     * @tparam C  The current character in the input string
     */
    template<typename S, typename C>
    struct rule
    {
        using type = reject;
    };

    // Specialized rules for specific inputs
    template<>
    struct rule<symbol::start, ast::character<'['>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_set,
                    symbol::set_begin,
                    symbol::expect<']'>,
                    symbol::mod,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::start, ast::character<'('>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::capture_begin,
                    symbol::expect<')'>,
                    symbol::make_capturing,
                    symbol::mod,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::start, ast::character<')'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::start, ast::character<'*'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::start, ast::character<'+'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::start, ast::character<'?'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::start, ast::character<'\\'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::esc,
                    symbol::mod,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::start, ast::character<'|'>>
    {
        using type = reject;
    };

    template<auto C>
    struct rule<symbol::start, ast::character<C>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_char,
                    symbol::mod,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::start, ast::character<'.'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_wildcard,
                    symbol::mod,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::start, ast::character<'^'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_beginning,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::start, ast::character<'$'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_ending,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::start, symbol::epsilon>
    {
        using type = ignore;
    };

    template<>
    struct rule<symbol::capture_begin, ast::character<'['>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_set,
                    symbol::set_begin,
                    symbol::expect<']'>,
                    symbol::mod,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::capture_begin, ast::character<'('>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::capture_begin,
                    symbol::expect<')'>,
                    symbol::make_capturing,
                    symbol::mod,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::capture_begin, ast::character<')'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::capture_begin, ast::character<'*'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::capture_begin, ast::character<'+'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::capture_begin, ast::character<'\\'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::esc,
                    symbol::mod,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::capture_begin, ast::character<'|'>>
    {
        using type = reject;
    };

    template<auto C>
    struct rule<symbol::capture_begin, ast::character<C>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_char,
                    symbol::mod,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::capture_begin, ast::character<'.'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_wildcard,
                    symbol::mod,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::capture_begin, ast::character<'^'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_beginning,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::capture_begin, symbol::epsilon>
    {
        using type = ignore;
    };

    template<>
    struct rule<symbol::capture_begin, ast::character<'?'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::capture_mod
                >;
    };

    template<>
    struct rule<symbol::capture_mod, ast::character<':'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::capture_begin_no_mod,
                    symbol::make_captureless
                >;
    };

    template<>
    struct rule<symbol::capture_mod, ast::character<'>'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::capture_begin_no_mod,
                    symbol::make_atomic
                >;
    };

    template<auto C>
    struct rule<symbol::capture_mod, ast::character<C>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::capture_begin_no_mod, ast::character<'['>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_set,
                    symbol::set_begin,
                    symbol::expect<']'>,
                    symbol::mod,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::capture_begin_no_mod, ast::character<'('>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::capture_begin,
                    symbol::expect<')'>,
                    symbol::make_capturing,
                    symbol::mod,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::capture_begin_no_mod, ast::character<')'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::capture_begin_no_mod, ast::character<'?'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::capture_begin_no_mod, ast::character<'*'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::capture_begin_no_mod, ast::character<'+'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::capture_begin_no_mod, ast::character<'\\'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::esc,
                    symbol::mod,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::capture_begin_no_mod, ast::character<'|'>>
    {
        using type = reject;
    };

    template<auto C>
    struct rule<symbol::capture_begin_no_mod, ast::character<C>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_char,
                    symbol::mod,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::capture_begin_no_mod, ast::character<'.'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_wildcard,
                    symbol::mod,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::capture_begin_no_mod, ast::character<'^'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_beginning,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::capture_begin_no_mod, symbol::epsilon>
    {
        using type = ignore;
    };

    template<>
    struct rule<symbol::alt, ast::character<')'>>
    {
        using type = ignore;
    };

    template<>
    struct rule<symbol::alt, ast::character<'|'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::seq0,
                    symbol::make_alternation,
                    symbol::alt
                >;
    };

    template<auto C>
    struct rule<symbol::alt, ast::character<C>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::alt, symbol::epsilon>
    {
        using type = ignore;
    };

    template<auto C>
    struct rule<symbol::esc, ast::character<C>>
    {
        using type = esc_rule_t<C>;
    };

    template<>
    struct rule<symbol::esc, symbol::epsilon>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::mod, ast::character<'*'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_star
                >;
    };

    template<>
    struct rule<symbol::mod, ast::character<'+'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_plus
                >;
    };

    template<>
    struct rule<symbol::mod, ast::character<'?'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_optional
                >;
    };

    template<>
    struct rule<symbol::mod, ast::character<'{'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::quantifier_begin
                >;
    };

    template<auto C>
    struct rule<symbol::quantifier_begin, ast::character<C>>
    {
        using type = begin_quantifier_value_t<C>;
    };

    template<>
    struct rule<symbol::quantifier_begin, symbol::epsilon>
    {
        using type = abort_quantifier_parsing_t<'{'>;
    };

    template<auto C>
    struct rule<symbol::mod, ast::character<C>>
    {
        using type = ignore;
    };

    template<>
    struct rule<symbol::mod, symbol::epsilon>
    {
        using type = ignore;
    };

    template<std::size_t N, auto C>
    struct rule<symbol::quantifier_value<N>, ast::character<C>>
    {
        using type =
                std::conditional_t
                <
                    is_numeric_v<C>,
                    stack
                    <
                        advance,
                        symbol::quantifier_value<10 * N + C - '0'>
                    >,
                    reject
                >;
    };

    template<std::size_t N>
    struct rule<symbol::quantifier_value<N>, ast::character<'}'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_repetition<symbol::quantifier_value<N>, symbol::quantifier_value<N>>
                >;
    };

    template<std::size_t N>
    struct rule<symbol::quantifier_value<N>, ast::character<','>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::quantifier_values<symbol::quantifier_value<N>, symbol::quantifier_inf>
                >;
    };

    template<typename A, typename B, auto C>
    struct rule<symbol::quantifier_values<A, B>, ast::character<C>>
    {
        using type =
                std::conditional_t
                <
                    is_numeric_v<C>,
                    stack
                    <
                        advance,
                        symbol::quantifier_values<A, update_quantifier_t<B, C>>
                    >,
                    reject
                >;
    };

    template<typename A, typename B>
    struct rule<symbol::quantifier_values<A, B>, ast::character<'}'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_repetition<A, B>
                >;
    };

    template<>
    struct rule<symbol::seq0, ast::character<'['>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_set,
                    symbol::set_begin,
                    symbol::expect<']'>,
                    symbol::mod,
                    symbol::seq
                >;
    };

    template<>
    struct rule<symbol::seq0, ast::character<'('>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::capture_begin,
                    symbol::expect<')'>,
                    symbol::make_capturing,
                    symbol::mod,
                    symbol::seq
                >;
    };

    template<>
    struct rule<symbol::seq0, ast::character<')'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::seq0, ast::character<'*'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::seq0, ast::character<'+'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::seq0, ast::character<'?'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::seq0, ast::character<'\\'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::esc,
                    symbol::mod,
                    symbol::seq
                >;
    };

    template<>
    struct rule<symbol::seq0, ast::character<'|'>>
    {
        using type = reject;
    };

    template<auto C>
    struct rule<symbol::seq0, ast::character<C>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_char,
                    symbol::mod,
                    symbol::seq
                >;
    };

    template<>
    struct rule<symbol::seq0, ast::character<'.'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_wildcard,
                    symbol::mod,
                    symbol::seq
                >;
    };

    template<>
    struct rule<symbol::seq0, ast::character<'^'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_beginning,
                    symbol::seq
                >;
    };

    template<>
    struct rule<symbol::seq0, ast::character<'$'>>
    {
        using type =
                stack
                <
                    advance,
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
    struct rule<symbol::seq, ast::character<'['>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_set,
                    symbol::set_begin,
                    symbol::expect<']'>,
                    symbol::mod,
                    symbol::make_sequence,
                    symbol::seq
                >;
    };

    template<>
    struct rule<symbol::seq, ast::character<'('>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::capture_begin,
                    symbol::expect<')'>,
                    symbol::make_capturing,
                    symbol::mod,
                    symbol::make_sequence,
                    symbol::seq
                >;
    };

    template<>
    struct rule<symbol::seq, ast::character<')'>>
    {
        using type = ignore;
    };

    template<>
    struct rule<symbol::seq, ast::character<'*'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::seq, ast::character<'+'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::seq, ast::character<'?'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::seq, ast::character<'\\'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::esc,
                    symbol::mod,
                    symbol::make_sequence,
                    symbol::seq
                >;
    };

    template<>
    struct rule<symbol::seq, ast::character<'|'>>
    {
        using type = ignore;
    };

    template<auto C>
    struct rule<symbol::seq, ast::character<C>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_char,
                    symbol::mod,
                    symbol::make_sequence,
                    symbol::seq
                >;
    };

    template<>
    struct rule<symbol::seq, ast::character<'.'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_wildcard,
                    symbol::mod,
                    symbol::make_sequence,
                    symbol::seq
                >;
    };

    template<>
    struct rule<symbol::seq, ast::character<'^'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_beginning,
                    symbol::make_sequence,
                    symbol::seq
                >;
    };

    template<>
    struct rule<symbol::seq, ast::character<'$'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_ending,
                    symbol::make_sequence,
                    symbol::seq
                >;
    };

    template<>
    struct rule<symbol::seq, symbol::epsilon>
    {
        using type = ignore;
    };

    // Rules for parsing sets
    template<>
    struct rule<symbol::set_begin, ast::character<'^'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::set_begin_no_neg,
                    symbol::make_negated
                >;
    };

    template<>
    struct rule<symbol::set_begin, ast::character<'\\'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::set_esc,
                    symbol::make_set_from_stack,
                    symbol::set_seq
                >;
    };

    template<>
    struct rule<symbol::set_begin, ast::character<']'>>
    {
        using type = ignore;
    };

    template<auto C>
    struct rule<symbol::set_begin, ast::character<C>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_set_from_current_char,
                    symbol::set_seq
                >;
    };

    template<>
    struct rule<symbol::set_begin_no_neg, ast::character<']'>>
    {
        using type = ignore;
    };

    template<>
    struct rule<symbol::set_begin_no_neg, ast::character<'\\'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::set_esc,
                    symbol::make_set_from_stack,
                    symbol::set_seq
                >;
    };

    template<auto C>
    struct rule<symbol::set_begin_no_neg, ast::character<C>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_set_from_current_char,
                    symbol::set_seq
                >;
    };

    template<>
    struct rule<symbol::set_seq, ast::character<']'>>
    {
        using type = ignore;
    };

    template<>
    struct rule<symbol::set_seq, ast::character<'\\'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::set_esc,
                    symbol::make_set_from_stack,
                    symbol::set_seq
                >;
    };

    template<>
    struct rule<symbol::set_seq, ast::character<'-'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::set_range_start,
                    symbol::set_seq
                >;
    };

    template<auto C>
    struct rule<symbol::set_seq, ast::character<C>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_set_from_current_char,
                    symbol::set_seq
                >;
    };

    template<auto C>
    struct rule<symbol::set_range_start, ast::character<C>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_range,
                    symbol::set_seq0
                >;
    };

    template<>
    struct rule<symbol::set_range_start, ast::character<']'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::set_range_start, ast::character<'\\'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::set_range_esc,
                    symbol::make_set_from_stack,
                    symbol::make_range_from_stack,
                    symbol::set_seq0
                >;
    };

    template<>
    struct rule<symbol::set_seq0, ast::character<'\\'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::set_esc,
                    symbol::make_set_from_stack,
                    symbol::set_seq
                >;
    };

    template<>
    struct rule<symbol::set_seq0, ast::character<']'>>
    {
        using type = ignore;
    };

    template<auto C>
    struct rule<symbol::set_seq0, ast::character<C>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_set_from_current_char,
                    symbol::set_seq
                >;
    };

    template<auto C>
    struct rule<symbol::set_esc, ast::character<C>>
    {
        using type = char_class_rule_t<C>;
    };

    template<auto C>
    struct rule<symbol::set_range_esc, ast::character<C>>
    {
        using type =
                stack
                <
                    advance,
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
    struct rule<symbol::backref_id<ID>, ast::character<C>>
    {
        using type = backref_rule_t<C, ID>;
    };

    template<std::size_t ID>
    struct rule<symbol::backref_id<ID>, symbol::epsilon>
    {
        using type = symbol::make_backref<ID>;
    };

    template<auto C>
    struct rule<symbol::expect<C>, ast::character<C>>
    {
        using type = advance;
    };

    template<>
    struct rule<empty_stack_marker, symbol::epsilon>
    {
        using type = accept;
    };

    template<typename A, typename B>
    using rule_t = typename rule<A, B>::type;
}
#endif //META_GRAMMAR_HPP