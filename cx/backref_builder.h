#ifndef CX_BACKREF_BUILDER_H
#define CX_BACKREF_BUILDER_H

#include "utility/stack.h"
#include "utility/char_helpers.h"
#include "symbol.h"
#include "astfwd.h"

namespace cx
{
    /**
     * Metafunction that decides if the current rule will continue
     * the backreference ID building or will finish it
     *
     * @tparam C    The current character in the regex pattern
     * @tparam ID   The current backreference ID on the stack
     */
    template<auto C, std::size_t ID, bool = is_numeric_v<C>>
    struct continue_backref_id
    {
        using type =
                stack
                <
                    character<C>,
                    symbol::backref_id<10 * ID + C - '0'>
                >;
    };

    template<auto C, std::size_t ID>
    struct continue_backref_id<C, ID, false>
    {
        using type = symbol::make_backref<ID>;
    };

    template<auto C, std::size_t ID>
    using continue_backref_id_t = typename continue_backref_id<C, ID>::type;
}
#endif //CX_BACKREF_BUILDER_H