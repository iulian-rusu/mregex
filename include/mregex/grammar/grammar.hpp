#ifndef MREGEX_GRAMMAR_HPP
#define MREGEX_GRAMMAR_HPP

#include <mregex/grammar/escapes.hpp>
#include <mregex/grammar/names.hpp>
#include <mregex/grammar/quantifiers.hpp>
#include <mregex/utility/type_traits.hpp>

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
                type_sequence
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
                type_sequence
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
    struct rule<symbol::begin, symbol::character<'{'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::begin, symbol::character<'\\'>>
    {
        using type =
                type_sequence
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
                type_sequence
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
                type_sequence
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
                type_sequence
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
                type_sequence
                <
                    advance,
                    symbol::make_end,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::begin, symbol::empty>
    {
        using type = ignore;
    };

    template<>
    struct rule<symbol::group_begin_or_mod, symbol::character<'['>>
    {
        using type =
                type_sequence
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
                type_sequence
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
        using type =
                type_sequence
                <
                    symbol::make_empty,
                    symbol::make_capture<symbol::unnamed>
                >;
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
    struct rule<symbol::group_begin_or_mod, symbol::character<'{'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::group_begin_or_mod, symbol::character<'\\'>>
    {
        using type =
                type_sequence
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
                type_sequence
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
                type_sequence
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
                type_sequence
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
                type_sequence
                <
                    advance,
                    symbol::make_end,
                    symbol::seq,
                    symbol::alt,
                    symbol::make_capture<symbol::unnamed>
                >;
    };

    template<>
    struct rule<symbol::group_begin_or_mod, symbol::empty>
    {
        using type = ignore;
    };

    template<>
    struct rule<symbol::group_begin_or_mod, symbol::character<'?'>>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::group_mod
                >;
    };

    template<>
    struct rule<symbol::group_mod, symbol::character<':'>>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::group_begin
                >;
    };

    template<>
    struct rule<symbol::group_mod, symbol::character<'<'>>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::group_mod_less
                >;
    };

    template<>
    struct rule<symbol::group_mod, symbol::character<'='>>
    {
        using type =
                type_sequence
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
                type_sequence
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
                type_sequence
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
                type_sequence
                <
                    advance,
                    symbol::group_begin,
                    symbol::make_negative_lookbehind
                >;
    };

    template<char C>
    struct rule<symbol::group_mod_less, symbol::character<C>>
    {
        using type = begin_name_t<symbol::capture_name_seq, C>;
    };

    template<char... Chars, char C>
    struct rule<symbol::capture_name_seq<Chars ...>, symbol::character<C>>
    {
        using type = update_name_t<symbol::capture_name_seq<Chars ...>, C>;
    };

    template<char... Chars>
    struct rule<symbol::capture_name_seq<Chars ...>, symbol::character<'>'>>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::group_begin,
                    symbol::make_capture<make_name<Chars ...>>
                >;
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
                type_sequence
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
                type_sequence
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
        using type = symbol::make_empty;
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
    struct rule<symbol::group_begin, symbol::character<'{'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::group_begin, symbol::character<'\\'>>
    {
        using type =
                type_sequence
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
                type_sequence
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
                type_sequence
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
                type_sequence
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
                type_sequence
                <
                    advance,
                    symbol::make_end,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<>
    struct rule<symbol::group_begin, symbol::empty>
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
                type_sequence
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
    struct rule<symbol::alt, symbol::empty>
    {
        using type = ignore;
    };

    template<char C>
    struct rule<symbol::esc, symbol::character<C>>
    {
        using type = begin_escape_sequence_t<C>;
    };

    template<>
    struct rule<symbol::mod, symbol::character<'*'>>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::quantifier_mod<symbol::make_star<match_mode::greedy>>
                >;
    };

    template<>
    struct rule<symbol::mod, symbol::character<'+'>>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::quantifier_mod<symbol::make_plus<match_mode::greedy>>
                >;
    };

    template<>
    struct rule<symbol::mod, symbol::character<'?'>>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::quantifier_mod<symbol::make_optional<match_mode::greedy>>
                >;
    };

    template<>
    struct rule<symbol::mod, symbol::character<'{'>>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::quantifier_begin
                >;
    };

    template<typename Token>
    struct rule<symbol::unquantifiable, Token>
    {
        using type = ignore;
    };

    template<>
    struct rule<symbol::unquantifiable, symbol::character<'*'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::unquantifiable, symbol::character<'+'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::unquantifiable, symbol::character<'?'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::unquantifiable, symbol::character<'{'>>
    {
        using type = reject;
    };

    template<char C>
    struct rule<symbol::quantifier_begin, symbol::character<C>>
    {
        using type = begin_quantifier_value_t<C>;
    };

    template<>
    struct rule<symbol::quantifier_begin, symbol::empty>
    {
        using type = abort_quantifier_parsing_t<'{'>;
    };

    template<typename Token>
    struct rule<symbol::mod, Token>
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
        using action = symbol::make_repetition<match_mode::greedy, symbol::quantifier_value<N>, symbol::quantifier_value<N>>;
        using type =
                type_sequence
                <
                    advance,
                    symbol::quantifier_mod<action>
                >;
    };

    template<std::size_t N>
    struct rule<symbol::quantifier_value<N>, symbol::character<','>>
    {
        using type =
                type_sequence
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
                type_sequence
                <
                    advance,
                    symbol::quantifier_mod<symbol::make_repetition<match_mode::greedy, A, B>>
                >;
    };

    template<template<match_mode, typename...> typename Action, match_mode Mode, typename... Inner>
    struct rule<symbol::quantifier_mod<Action<Mode, Inner ...>>, symbol::character<'?'>>
    {
        using type =
                type_sequence
                <
                    advance,
                    Action<match_mode::lazy, Inner ...>
                >;
    };

    template<template<match_mode, typename...> typename Action, match_mode Mode, typename... Inner>
    struct rule<symbol::quantifier_mod<Action<Mode, Inner ...>>, symbol::character<'+'>>
    {
        using type =
                type_sequence
                <
                    advance,
                    Action<match_mode::possessive, Inner ...>
                >;
    };

    template<typename Action, typename Token>
    struct rule<symbol::quantifier_mod<Action>, Token>
    {
        using type = Action;
    };

    template<>
    struct rule<symbol::alt_seq, symbol::character<'['>>
    {
        using type =
                type_sequence
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
                type_sequence
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
    struct rule<symbol::alt_seq, symbol::character<'{'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::alt_seq, symbol::character<'\\'>>
    {
        using type =
                type_sequence
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
                type_sequence
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
                type_sequence
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
                type_sequence
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
                type_sequence
                <
                    advance,
                    symbol::make_end,
                    symbol::seq
                >;
    };

    template<>
    struct rule<symbol::alt_seq, symbol::empty>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::seq, symbol::character<'['>>
    {
        using type =
                type_sequence
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
                type_sequence
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
    struct rule<symbol::seq, symbol::character<'{'>>
    {
        using type = reject;
    };

    template<>
    struct rule<symbol::seq, symbol::character<'\\'>>
    {
        using type =
                type_sequence
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
                type_sequence
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
                type_sequence
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
                type_sequence
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
                type_sequence
                <
                    advance,
                    symbol::make_end,
                    symbol::make_sequence,
                    symbol::seq
                >;
    };

    template<>
    struct rule<symbol::seq, symbol::empty>
    {
        using type = ignore;
    };

    // Rules for parsing sets
    template<>
    struct rule<symbol::set_begin_or_neg, symbol::character<'^'>>
    {
        using type =
                type_sequence
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
                type_sequence
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
                type_sequence
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
                type_sequence
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
                type_sequence
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
                type_sequence
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
                type_sequence
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
                type_sequence
                <
                    advance,
                    symbol::make_set_from_current_char,
                    symbol::set_seq
                >;
    };

    template<char C>
    struct rule<symbol::set_esc, symbol::character<C>>
    {
        using type = begin_set_escape_sequence_t<C>;
    };

    template<char C>
    struct rule<symbol::set_range_begin, symbol::character<C>>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_range,
                    symbol::set_range_seq
                >;
    };

    template<>
    struct rule<symbol::set_range_begin, symbol::character<']'>>
    {
        using type =
                type_sequence
                <
                    symbol::push_literal<'-'>,
                    symbol::make_set_from_stack
                >;
    };

    template<>
    struct rule<symbol::set_range_begin, symbol::character<'\\'>>
    {
        using type =
                type_sequence
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
                type_sequence
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
                type_sequence
                <
                    advance,
                    symbol::make_set_from_current_char,
                    symbol::set_seq
                >;
    };

    template<char C>
    struct rule<symbol::set_range_esc, symbol::character<C>>
    {
        using type = begin_set_range_escape_sequence_t<C>;
    };

    template<>
    struct rule<symbol::set_range_esc, symbol::empty>
    {
        using type = reject;
    };

    // Rules for parsing identifier-based backreferences
    template<std::size_t ID, char C>
    struct rule<symbol::backref_id<ID>, symbol::character<C>>
    {
        using type = update_backref_t<ID, C>;
    };

    template<std::size_t ID>
    struct rule<symbol::backref_id<ID>, symbol::empty>
    {
        using type = symbol::make_backref<ID>;
    };

    // Rules for parsing name-based backreferences
    template<char C>
    struct rule<symbol::backref_name_begin, symbol::character<C>>
    {
        using type = begin_name_t<symbol::backref_name_seq, C>;
    };

    template<char... Chars, char C>
    struct rule<symbol::backref_name_seq<Chars ...>, symbol::character<C>>
    {
        using type = update_name_t<symbol::backref_name_seq<Chars ...>, C>;
    };

    template<char... Chars>
    struct rule<symbol::backref_name_seq<Chars ...>, symbol::character<'>'>>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::make_named_backref<make_name<Chars ...>>
                >;
    };

    template<char C>
    struct rule<symbol::expect<C>, symbol::character<C>>
    {
        using type = advance;
    };

    template<>
    struct rule<symbol::empty, symbol::empty>
    {
        using type = accept;
    };

    template<typename Symbol, typename Token>
    using rule_t = typename rule<Symbol, Token>::type;
}
#endif //MREGEX_GRAMMAR_HPP