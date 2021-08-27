#ifndef META_REGEX_UNION_HPP
#define META_REGEX_UNION_HPP

#include "parser.hpp"
#include "regex_base.hpp"

namespace meta
{
    /**
     * Interface for creating an efficient alternation of several regex patterns.
     *
     * @tparam first    The first pattern in the union
     * @tparam rest     The remaining union patterns
     */
    template<static_string first, static_string... rest>
    struct regex_union : regex_base<ast::disjunction<ast_of<first>, ast_of<rest> ...>>
    {
        static_assert(
                (parser<first>::accepted && (parser<rest>::accepted && ...)),
                "at least one regular expression in the union contains a syntax error"
        );
    };

    template<typename R, typename... Rs>
    using make_union = regex_base<ast::disjunction<typename R::ast_type, typename Rs::ast_type ...>>;
}
#endif //META_REGEX_UNION_HPP