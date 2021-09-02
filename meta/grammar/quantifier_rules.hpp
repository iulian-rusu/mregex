#ifndef META_QUANTIFIER_RULES_HPP
#define META_QUANTIFIER_RULES_HPP

#include "grammar_actions.hpp"
#include "../utility/stack.hpp"
#include "../utility/char_traits.hpp"
#include "../symbol.hpp"

namespace meta::grammar
{
    /**
     * Metafunction used as a grammar rule when the parsing of a quantifier fails.
     * In this case, the parser has to recover the skipped metacharacter used to
     * mark the start of a quantifier, like '{'.
     *
     * @tparam C    The character that is parsed as replacement for the failed quantifier
     */
    template<auto C>
    struct abort_quantifier_parsing
    {
        using type =
                stack
                <
                    symbol::push_literal<C>,
                    symbol::mod,
                    symbol::make_sequence,
                    symbol::seq,
                    symbol::alt
                >;
    };

    template<auto C>
    using abort_quantifier_parsing_t = typename abort_quantifier_parsing<C>::type;

    /**
     * Metafunction that decides if the parser will begin parsing a quantifier value
     * or a normal character sequence.
     *
     * @tparam C    The current character being parsed
     */
    template<auto C, bool = is_numeric_v<C>>
    struct begin_quantifier_value
    {
        using type =
                stack
                <
                    advance,
                    symbol::quantifier_value<C - '0'>
                >;
    };
    
    template<auto C>
    struct begin_quantifier_value<C, false>
    {
        using type = abort_quantifier_parsing_t<'{'>;
    };

    template<auto C>
    using begin_quantifier_value_t = typename begin_quantifier_value<C>::type;

    /**
     * Metafunction that updates a given quantifier value which might also be infinite.
     * Base 10 number parsing is assumed, the absence of a value implies infinity.
     *
     * @tparam T    The quantifier value symbol to be updated
     * @tparam C    The current character being parsed
     */
    template<typename T, auto C, bool = is_numeric_v<C>>
    struct update_quantifier;

    template<auto A, auto C>
    struct update_quantifier<symbol::quantifier_value<A>, C, true>
    {
        using type = symbol::quantifier_value<10 * A + C - '0'>;
    };

    template<auto C>
    struct update_quantifier<symbol::quantifier_inf, C, true>
    {
        using type = symbol::quantifier_value<C - '0'>;
    };

    template<typename T, auto C>
    struct update_quantifier<T, C, false>
    {
        using type = T;
    };

    template<typename T, auto C>
    using update_quantifier_t = typename update_quantifier<T, C>::type;
}
#endif //META_QUANTIFIER_RULES_HPP