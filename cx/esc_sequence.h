#ifndef CX_ESC_SEQUENCE_H
#define CX_ESC_SEQUENCE_H

#include "utility/stack.h"
#include "symbol.h"
#include "astfwd.h"
#include "char_class.h"
#include "backref_builder.h"

namespace cx::grammar
{
    /**
     * Metafunction that decides the rule which handles an escaped character
     * or sequence in the input being parsed
     *
     * @tparam C    The current character in the input pattern
     */
    template<auto C, bool = C != '0' && is_numeric_v<C>>
    struct esc_rule
    {
        using type =
                stack
                <
                    character<C>,
                    symbol::backref_id<C - '0'>
                >;
    };

    template<>
    struct esc_rule<'n', false>
    {
        using type =
                stack
                <
                    character<'n'>,
                    symbol::make_newline
                >;
    };

    template<auto C>
    struct esc_rule<C, false>
    {
        using type = char_class_rule_t<C>;
    };

    template<auto C>
    using esc_rule_t = typename esc_rule<C>::type;
}
#endif //CX_ESC_SEQUENCE_H