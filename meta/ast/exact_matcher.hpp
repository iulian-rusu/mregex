#ifndef META_EXACT_MATCHER_HPP
#define META_EXACT_MATCHER_HPP

#include "astfwd.hpp"

namespace meta::ast
{
    /**
     * Appends an assertion that checks for the end of the input sequence.
     *
     * @tparam AST  The Abstract Syntax Tree of the regular expression
     */
    template<typename AST>
    struct enforce_exact_match
    {
        using type = sequence<AST, endsequence>;
    };

    template<typename AST>
    using enforce_exact_match_t = typename enforce_exact_match<AST>::type;

    template<typename... Elems>
    struct enforce_exact_match<sequence<Elems ...>>
    {
        using type = sequence<Elems ..., endsequence>;
    };

    template<typename... Elems>
    struct enforce_exact_match<alternation<Elems ...>>
    {
        using type = alternation<enforce_exact_match_t<Elems> ...>;
    };

    template<typename... Elems>
    struct enforce_exact_match<disjunction<Elems ...>>
    {
        using type = disjunction<enforce_exact_match_t<Elems> ...>;
    };

    /**
     * Modifies the AST to either exactly match any given input sequence or return false.
     * This is needed because the matching algorithm is greedy and returns the first found
     * match which may consume less characters than required.
     *
     * @tparam AST  The Abstract Syntax Tree of the regular expression
     */
    template<typename AST>
    struct exact_matcher
    {
        template<typename... Args>
        static constexpr auto match(Args &&... args) noexcept
        {
            return enforce_exact_match_t<AST>::match(args ...);
        }
    };
}
#endif //META_EXACT_MATCHER_HPP
