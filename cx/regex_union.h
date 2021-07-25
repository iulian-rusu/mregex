#ifndef CX_REGEX_UNION_H
#define CX_REGEX_UNION_H

#include "parser.h"
#include "regex_base.h"

namespace cx
{
    /**
     * Type that represents a convenient interface for creating unions of multiple Regexes
     *
     * @tparam first    The first pattern in the union
     * @tparam rest     The remaining union patterns
     */
    template<static_string const first, static_string const ... rest>
    struct regex_union : regex_base<disjunction<get_ast<first>, get_ast<rest> ...>>
    {
        static_assert(
            (parser<first>::accepted && (parser<rest>::accepted && ... )),
            "at least one regular expression in the union contains a syntax error"
        );
    };

    template<typename R, typename ... Rs>
    using make_union = regex_base<disjunction<typename R::ast, typename Rs::ast ...>>;
}
#endif //CX_REGEX_UNION_H
