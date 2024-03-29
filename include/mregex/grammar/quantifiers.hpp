#ifndef MREGEX_GRAMMAR_QUANTIFIERS_HPP
#define MREGEX_GRAMMAR_QUANTIFIERS_HPP

#include <mregex/grammar/actions.hpp>
#include <mregex/utility/char_traits.hpp>
#include <mregex/utility/type_sequence.hpp>

namespace meta::grammar
{
    /**
     * Metafunction that defines symbols used to abort parsing a quantifier.
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
    template<char C>
    struct begin_quantifier_value
    {
        using type = abort_quantifier_parsing_t<'{'>;
    };

    template<char C>
    requires (is_digit(C))
    struct begin_quantifier_value<C>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::quantifier_value<C - '0'>
                >;
    };

    template<char C>
    using begin_quantifier_value_t = typename begin_quantifier_value<C>::type;

    /**
     * Metafunction that defines symbols used to continue parsing quantifier values.
     *
     * @tparam Symbol   The symbolic quantifier being parsed
     * @tparam C        The current character being parsed
     */
    template<typename Symbol, char C>
    struct continue_quantifier_value
    {
        using type = reject;
    };

    template<std::size_t N, char C>
    requires (is_digit(C))
    struct continue_quantifier_value<symbol::quantifier_value<N>, C>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::quantifier_value<10 * N + C - '0'>
                >;
    };

    template<symbol::quantifier A, std::size_t N, char C>
    requires (is_digit(C))
    struct continue_quantifier_value<symbol::quantifier_range<A, symbol::quantifier_value<N>>, C>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::quantifier_range<A, symbol::quantifier_value<10 * N + C - '0'>>
                >;
    };

    template<symbol::quantifier A, char C>
    requires (is_digit(C))
    struct continue_quantifier_value<symbol::quantifier_range<A, symbol::infinity>, C>
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::quantifier_range<A, symbol::quantifier_value<C - '0'>>
                >;
    };

    template<typename Symbol, char C>
    using continue_quantifier_value_t = typename continue_quantifier_value<Symbol, C>::type;
}
#endif //MREGEX_GRAMMAR_QUANTIFIERS_HPP