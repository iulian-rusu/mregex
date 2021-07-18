#ifndef CX_QUANTIFIER_BUILDER_H
#define CX_QUANTIFIER_BUILDER_H

#include "utility/stack.h"
#include "utility/char_helpers.h"
#include "symbol.h"
#include "ast_fwd.h"

namespace cx
{

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
        using type =
                stack
                <
                    symbol::push_char<'{'>,
                    symbol::mod,
                    symbol::seq,
                    symbol::make_sequence,
                    symbol::alt
                >;
    };

    template<auto C>
    using begin_quantifier_value_t = typename begin_quantifier_value<C>::type;
}
#endif //CX_QUANTIFIER_BUILDER_H
