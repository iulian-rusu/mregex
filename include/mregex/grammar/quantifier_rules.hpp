#ifndef MREGEX_QUANTIFIER_RULES_HPP
#define MREGEX_QUANTIFIER_RULES_HPP

#include <mregex/grammar/grammar_actions.hpp>
#include <mregex/utility/char_traits.hpp>
#include <mregex/utility/type_sequence.hpp>

namespace meta::grammar
{
    /**
     * Metafunction used as a grammar rule when the parsing of a quantifier fails.
     * In this case, the parser has to recover the skipped metacharacter used to
     * mark the start of a quantifier, like '{'.
     *
     * @tparam C    The character that is parsed as replacement for the failed quantifier
     */
    template<char C>
    struct abort_quantifier_parsing
    {
        using type =
                type_sequence
                <
                    symbol::push_literal<C>,
                    symbol::mod,
                    symbol::make_sequence,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<char C>
    using abort_quantifier_parsing_t = typename abort_quantifier_parsing<C>::type;

    /**
     * Metafunction that decides if the parser will begin parsing a quantifier value
     * or a normal character sequence.
     *
     * @tparam C    The current character being parsed
     */
    template<char C, bool = is_numeric_v<C>>
    struct begin_quantifier_value
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::quantifier_value<C - '0'>
                >;
    };
    
    template<char C>
    struct begin_quantifier_value<C, false>
    {
        using type = abort_quantifier_parsing_t<'{'>;
    };

    template<char C>
    using begin_quantifier_value_t = typename begin_quantifier_value<C>::type;

    /**
     * Metafunction that defines the rules for updating quantifier values when parsing a new token.
     *
     * @tparam T    The quantifier value symbol being updated
     * @tparam C    The current character being parsed
     */
    template<typename T, char C, bool = is_numeric_v<C>>
    struct update_quantifier_value;

    template<auto N, char C>
    struct update_quantifier_value<symbol::quantifier_value<N>, C, true>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::quantifier_value<10 * N + C - '0'>
                >;
    };

    template<typename T, auto N, char C>
    struct update_quantifier_value<symbol::quantifier_values<T, symbol::quantifier_value<N>>, C, true>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::quantifier_values<T, symbol::quantifier_value<10 * N + C - '0'>>
                >;
    };

    template<typename T, char C>
    struct update_quantifier_value<symbol::quantifier_values<T, symbol::quantifier_inf>, C, true>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::quantifier_values<T, symbol::quantifier_value<C - '0'>>
                >;
    };

    template<typename T, char C>
    struct update_quantifier_value<T, C, false>
    {
        using type = reject;
    };

    template<typename T, char C>
    using update_quantifier_value_t = typename update_quantifier_value<T, C>::type;
}
#endif //MREGEX_QUANTIFIER_RULES_HPP