#ifndef MREGEX_REGEX_UNION_HPP
#define MREGEX_REGEX_UNION_HPP

#include <mregex/parsing/parser.hpp>
#include <mregex/regex_base.hpp>

namespace meta
{
    /**
     * Interface for creating a union of several regex patterns.
     * Unlike an alternation, regexes in a union behave as independent expressions.
     * That is, their captures are indexed separately and the regex
     * matching context is cleared after evaluating each sub-option.
     *
     * @tparam First    The first pattern in the union
     * @tparam Rest     The remaining union patterns
     */
    template<static_string First, static_string... Rest>
    struct regex_union : regex_base<ast::disjunction<ast_of<First>, ast_of<Rest> ...>>
    {
        static_assert(
            (parser<First>::accepted && (parser<Rest>::accepted && ...)),
            "syntax error in at least one regular expression"
        );
    };

    template<typename First, typename... Rest>
    using make_union = regex_base<ast::disjunction<typename First::ast_type, typename Rest::ast_type ...>>;
}
#endif //MREGEX_REGEX_UNION_HPP