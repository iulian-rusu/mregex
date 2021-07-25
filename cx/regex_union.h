#ifndef CX_REGEX_UNION_H
#define CX_REGEX_UNION_H

#include "parser.h"
#include "regex_base.h"

namespace cx
{
    /**
     * Interface for creating unions of several Regexes
     *
     * @tparam first    The first pattern in the union
     * @tparam rest     The remaining union patterns
     */
    template<static_string const first, static_string const ... rest>
    struct regex_union : regex_base<disjunction<ast_of<first>, ast_of<rest> ...>>
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