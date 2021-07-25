#ifndef CX_QUANTIFIER_BUILDER_H
#define CX_QUANTIFIER_BUILDER_H

#include "utility/stack.h"
#include "utility/char_helpers.h"
#include "symbol.h"
#include "astfwd.h"

namespace cx
{
    /**
     * Metafunction used as a grammar rule when the parsing of a quantifier fails.
     * In this case, the parser has to recover the skipped metacharacter used to
     * identify the start of a quantifier, like '{'
     *
     * @tparam C    The character that is parsed as replacement for the failed quantifier
     */
    template<auto C>
    struct abort_quantifier_parsing
    {
        using type =
                stack
                <
                    symbol::push_char<C>,
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
     * or will parse it as a normal character sequence
     *
     * @tparam C    The current character being parsed
     */
    template<auto C, bool = is_numeric_v<C>>
    struct begin_quantifier_value
    {
        using type =
                stack
                <
                    character<C>,
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
}
#endif //CX_QUANTIFIER_BUILDER_H