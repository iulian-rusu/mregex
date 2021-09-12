#ifndef MREGEX_REGEX_UNION_HPP
#define MREGEX_REGEX_UNION_HPP

#include <mregex/parser.hpp>
#include <mregex/regex_base.hpp>

namespace meta
{
    /**
     * Interface for creating a union of several regex patterns.
     * Unlike an alternation, regexes in a union behave as independent expressions.
     * That is, their captures are indexed separately and the regex
     * matching context is cleared after evaluating each sub-option.
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

    template<typename First, typename... Rest>
    using make_union = regex_base<ast::disjunction<typename First::ast_type, typename Rest::ast_type ...>>;
}
#endif //MREGEX_REGEX_UNION_HPP