#ifndef MREGEX_GRAMMAR_HPP
#define MREGEX_GRAMMAR_HPP

#include <mregex/grammar/esc_rules.hpp>
#include <mregex/grammar/quantifier_rules.hpp>

namespace meta::grammar
{
    /**
     * Metafunction defining a grammar rule based on two parameters.
     *
     * @tparam Symbol  The current symbol on the stack
     * @tparam Token   The current token being parsed
     */
    template<typename Symbol, typename Token>
    struct rule
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::begin, symbol::character<'['>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_set,
                    symbol::set_begin_or_neg,
                    symbol::expect<']'>,
                    symbol::mod,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::begin, symbol::character<'('>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::group_begin_or_mod,
                    symbol::expect<')'>,
                    symbol::mod,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::begin, symbol::character<')'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::begin, symbol::character<'*'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::begin, symbol::character<'+'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::begin, symbol::character<'?'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::begin, symbol::character<'\\'>>
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
    struct rule<symbol::begin, symbol::character<'|'>>
    {
        using type = reject;
    };

    template<char C>
    struct rule<symbol::begin, symbol::character<C>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_literal,
                    symbol::mod,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::begin, symbol::character<'.'>>
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
    struct rule<symbol::begin, symbol::character<'^'>>
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
    struct rule<symbol::begin, symbol::character<'$'>>
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
    struct rule<symbol::begin, symbol::epsilon>
    {
        using type = ignore;
    };

    template<>
    struct rule<symbol::group_begin_or_mod, symbol::character<'['>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_set,
                    symbol::set_begin_or_neg,
                    symbol::expect<']'>,
                    symbol::mod,
                    symbol::seq,
                    symbol::alt,
                    symbol::make_capture<symbol::unnamed>
                >;
    };

    template<>
    struct rule<symbol::group_begin_or_mod, symbol::character<'('>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::group_begin_or_mod,
                    symbol::expect<')'>,
                    symbol::mod,
                    symbol::seq,
                    symbol::alt,
                    symbol::make_capture<symbol::unnamed>
                >;
    };

    template<>
    struct rule<symbol::group_begin_or_mod, symbol::character<')'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::group_begin_or_mod, symbol::character<'*'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::group_begin_or_mod, symbol::character<'+'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::group_begin_or_mod, symbol::character<'\\'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::esc,
                    symbol::mod,
                    symbol::seq,
                    symbol::alt,
                    symbol::make_capture<symbol::unnamed>
                >;
    };

    template<>
    struct rule<symbol::group_begin_or_mod, symbol::character<'|'>>
    {
        using type = reject;
    };

    template<char C>
    struct rule<symbol::group_begin_or_mod, symbol::character<C>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_literal,
                    symbol::mod,
                    symbol::seq,
                    symbol::alt,
                    symbol::make_capture<symbol::unnamed>
                >;
    };

    template<>
    struct rule<symbol::group_begin_or_mod, symbol::character<'.'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_wildcard,
                    symbol::mod,
                    symbol::seq,
                    symbol::alt,
                    symbol::make_capture<symbol::unnamed>
                >;
    };

    template<>
    struct rule<symbol::group_begin_or_mod, symbol::character<'^'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_beginning,
                    symbol::seq,
                    symbol::alt,
                    symbol::make_capture<symbol::unnamed>
                >;
    };

    template<>
    struct rule<symbol::group_begin_or_mod, symbol::character<'$'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_ending,
                    symbol::seq,
                    symbol::alt,
                    symbol::make_capture<symbol::unnamed>
                >;
    };

    template<>
    struct rule<symbol::group_begin_or_mod, symbol::epsilon>
    {
        using type = ignore;
    };

    template<>
    struct rule<symbol::group_begin_or_mod, symbol::character<'?'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::group_mod
                >;
    };

    template<>
    struct rule<symbol::group_mod, symbol::character<':'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::group_begin
                >;
    };

    template<>
    struct rule<symbol::group_mod, symbol::character<'<'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::group_mod_less
                >;
    };

    template<>
    struct rule<symbol::group_mod, symbol::character<'='>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::group_begin,
                    symbol::make_positive_lookahead
                >;
    };

    template<>
    struct rule<symbol::group_mod, symbol::character<'!'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::group_begin,
                    symbol::make_negative_lookahead
                >;
    };

    template<>
    struct rule<symbol::group_mod_less, symbol::character<'='>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::group_begin,
                    symbol::make_positive_lookbehind
                >;
    };

    template<>
    struct rule<symbol::group_mod_less, symbol::character<'!'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::group_begin,
                    symbol::make_negative_lookbehind
                >;
    };

    template<char C>
    struct rule<symbol::group_mod_less, symbol::character<C>>
    {
        // TODO: add rules for parsing capture names here
        using type = reject;
    };

    template<char C>
    struct rule<symbol::group_mod, symbol::character<C>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::group_begin, symbol::character<'['>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_set,
                    symbol::set_begin_or_neg,
                    symbol::expect<']'>,
                    symbol::mod,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::group_begin, symbol::character<'('>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::group_begin_or_mod,
                    symbol::expect<')'>,
                    symbol::mod,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::group_begin, symbol::character<')'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::group_begin, symbol::character<'?'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::group_begin, symbol::character<'*'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::group_begin, symbol::character<'+'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::group_begin, symbol::character<'\\'>>
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
    struct rule<symbol::group_begin, symbol::character<'|'>>
    {
        using type = reject;
    };

    template<char C>
    struct rule<symbol::group_begin, symbol::character<C>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_literal,
                    symbol::mod,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::group_begin, symbol::character<'.'>>
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
    struct rule<symbol::group_begin, symbol::character<'^'>>
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
    struct rule<symbol::group_begin, symbol::character<'$'>>
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
    struct rule<symbol::group_begin, symbol::epsilon>
    {
        using type = ignore;
    };

    template<>
    struct rule<symbol::alt, symbol::character<')'>>
    {
        using type = ignore;
    };

    template<>
    struct rule<symbol::alt, symbol::character<'|'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::alt_seq,
                    symbol::make_alternation,
                    symbol::alt
                >;
    };

    template<char C>
    struct rule<symbol::alt, symbol::character<C>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::alt, symbol::epsilon>
    {
        using type = ignore;
    };

    template<char C>
    struct rule<symbol::esc, symbol::character<C>>
    {
        using type = esc_rule_t<C>;
    };

    template<>
    struct rule<symbol::esc, symbol::epsilon>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::mod, symbol::character<'*'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_star
                >;
    };

    template<>
    struct rule<symbol::mod, symbol::character<'+'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_plus
                >;
    };

    template<>
    struct rule<symbol::mod, symbol::character<'?'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_optional
                >;
    };

    template<>
    struct rule<symbol::mod, symbol::character<'{'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::quantifier_begin
                >;
    };

    template<char C>
    struct rule<symbol::quantifier_begin, symbol::character<C>>
    {
        using type = begin_quantifier_value_t<C>;
    };

    template<>
    struct rule<symbol::quantifier_begin, symbol::epsilon>
    {
        using type = abort_quantifier_parsing_t<'{'>;
    };

    template<char C>
    struct rule<symbol::mod, symbol::character<C>>
    {
        using type = ignore;
    };

    template<>
    struct rule<symbol::mod, symbol::epsilon>
    {
        using type = ignore;
    };

    template<std::size_t N, char C>
    struct rule<symbol::quantifier_value<N>, symbol::character<C>>
    {
        using type = update_quantifier_value_t<symbol::quantifier_value<N>, C>;
    };

    template<std::size_t N>
    struct rule<symbol::quantifier_value<N>, symbol::character<'}'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_repetition<symbol::quantifier_value<N>, symbol::quantifier_value<N>>
                >;
    };

    template<std::size_t N>
    struct rule<symbol::quantifier_value<N>, symbol::character<','>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::quantifier_values<symbol::quantifier_value<N>, symbol::quantifier_inf>
                >;
    };

    template<typename A, typename B, char C>
    struct rule<symbol::quantifier_values<A, B>, symbol::character<C>>
    {
        using type = update_quantifier_value_t<symbol::quantifier_values<A, B>, C>;
    };

    template<typename A, typename B>
    struct rule<symbol::quantifier_values<A, B>, symbol::character<'}'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_repetition<A, B>
                >;
    };

    template<>
    struct rule<symbol::alt_seq, symbol::character<'['>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_set,
                    symbol::set_begin_or_neg,
                    symbol::expect<']'>,
                    symbol::mod,
                    symbol::seq
                >;
    };

    template<>
    struct rule<symbol::alt_seq, symbol::character<'('>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::group_begin_or_mod,
                    symbol::expect<')'>,
                    symbol::mod,
                    symbol::seq
                >;
    };

    template<>
    struct rule<symbol::alt_seq, symbol::character<')'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::alt_seq, symbol::character<'*'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::alt_seq, symbol::character<'+'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::alt_seq, symbol::character<'?'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::alt_seq, symbol::character<'\\'>>
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
    struct rule<symbol::alt_seq, symbol::character<'|'>>
    {
        using type = reject;
    };

    template<char C>
    struct rule<symbol::alt_seq, symbol::character<C>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_literal,
                    symbol::mod,
                    symbol::seq
                >;
    };

    template<>
    struct rule<symbol::alt_seq, symbol::character<'.'>>
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
    struct rule<symbol::alt_seq, symbol::character<'^'>>
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
    struct rule<symbol::alt_seq, symbol::character<'$'>>
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
    struct rule<symbol::alt_seq, symbol::epsilon>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::seq, symbol::character<'['>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_set,
                    symbol::set_begin_or_neg,
                    symbol::expect<']'>,
                    symbol::mod,
                    symbol::make_sequence,
                    symbol::seq
                >;
    };

    template<>
    struct rule<symbol::seq, symbol::character<'('>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::group_begin_or_mod,
                    symbol::expect<')'>,
                    symbol::mod,
                    symbol::make_sequence,
                    symbol::seq
                >;
    };

    template<>
    struct rule<symbol::seq, symbol::character<')'>>
    {
        using type = ignore;
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
    struct rule<symbol::seq, symbol::character<'|'>>
    {
        using type = ignore;
    };

    template<char C>
    struct rule<symbol::seq, symbol::character<C>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_literal,
                    symbol::mod,
                    symbol::make_sequence,
                    symbol::seq
                >;
    };

    template<>
    struct rule<symbol::seq, symbol::character<'.'>>
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
    struct rule<symbol::seq, symbol::character<'^'>>
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
    struct rule<symbol::seq, symbol::character<'$'>>
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
    struct rule<symbol::set_begin_or_neg, symbol::character<'^'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::set_begin,
                    symbol::make_negated
                >;
    };

    template<>
    struct rule<symbol::set_begin_or_neg, symbol::character<'\\'>>
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
    struct rule<symbol::set_begin_or_neg, symbol::character<']'>>
    {
        using type = ignore;
    };

    template<char C>
    struct rule<symbol::set_begin_or_neg, symbol::character<C>>
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
    struct rule<symbol::set_begin, symbol::character<']'>>
    {
        using type = ignore;
    };

    template<>
    struct rule<symbol::set_begin, symbol::character<'\\'>>
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

    template<char C>
    struct rule<symbol::set_begin, symbol::character<C>>
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
    struct rule<symbol::set_seq, symbol::character<']'>>
    {
        using type = ignore;
    };

    template<>
    struct rule<symbol::set_seq, symbol::character<'\\'>>
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
    struct rule<symbol::set_seq, symbol::character<'-'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::set_range_begin,
                    symbol::set_seq
                >;
    };

    template<char C>
    struct rule<symbol::set_seq, symbol::character<C>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_set_from_current_char,
                    symbol::set_seq
                >;
    };

    template<char C>
    struct rule<symbol::set_range_begin, symbol::character<C>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_range,
                    symbol::set_range_seq
                >;
    };

    template<>
    struct rule<symbol::set_range_begin, symbol::character<']'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::set_range_begin, symbol::character<'\\'>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::set_range_esc,
                    symbol::make_set_from_stack,
                    symbol::make_range_from_stack,
                    symbol::set_range_seq
                >;
    };

    template<>
    struct rule<symbol::set_range_seq, symbol::character<'\\'>>
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
    struct rule<symbol::set_range_seq, symbol::character<']'>>
    {
        using type = ignore;
    };

    template<char C>
    struct rule<symbol::set_range_seq, symbol::character<C>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_set_from_current_char,
                    symbol::set_seq
                >;
    };

    template<char C>
    struct rule<symbol::set_esc, symbol::character<C>>
    {
        using type = esc_rule_t<C>;
    };

    template<char C>
    struct rule<symbol::set_range_esc, symbol::character<C>>
    {
        using type =
                stack
                <
                    advance,
                    symbol::make_literal
                >;
    };

    template<typename Token>
    struct rule<symbol::set_range_esc, Token>
    {
        using type = reject;
    };

    // Rules for parsing backreferences
    template<std::size_t ID, char C>
    struct rule<symbol::backref_id<ID>, symbol::character<C>>
    {
        using type = backref_rule_t<C, ID>;
    };

    template<std::size_t ID>
    struct rule<symbol::backref_id<ID>, symbol::epsilon>
    {
        using type = symbol::make_backref<ID>;
    };

    template<char C>
    struct rule<symbol::expect<C>, symbol::character<C>>
    {
        using type = advance;
    };

    template<>
    struct rule<empty_stack_marker, symbol::epsilon>
    {
        using type = accept;
    };

    template<typename Symbol, typename Token>
    using rule_t = typename rule<Symbol, Token>::type;
}
#endif //MREGEX_GRAMMAR_HPP