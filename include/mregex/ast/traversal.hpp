#ifndef MREGEX_AST_TRAVERSAL_HPP
#define MREGEX_AST_TRAVERSAL_HPP

#include <mregex/ast/astfwd.hpp>

namespace meta::ast
{
    /**
     * Generic metafunction that implements boilerplate code for AST traverse.
     *
     * @tparam Transform    Metafunction that provides a mapping between AST nodes
     * @tparam Node         The AST node type being visited
     */
    template<template<typename> typename Transform, typename Node>
    struct traverse
    {
        using type = Node;
    };

    template<template<typename> typename Transform, template<typename...> typename Wrapper, typename... Inner>
    struct traverse<Transform, Wrapper<Inner ...>>
    {
        using type = Wrapper<typename Transform<Inner>::type ...>;
    };

    template<template<typename> typename Transform, match_mode Mode, symbol::finite_quantifier A, symbol::quantifier B, typename Inner>
    struct traverse<Transform, basic_repetition<Mode, A, B, Inner>>
    {
        using type = basic_repetition<Mode, A, B, typename Transform<Inner>::type>;
    };

    template<template<typename> typename Transform, template<assertion_mode, lookaround_direction, typename> typename Lookaround, assertion_mode Mode, lookaround_direction Direction, typename Inner>
    struct traverse<Transform, Lookaround<Mode, Direction, Inner>>
    {
        using type = Lookaround<Mode, Direction, typename Transform<Inner>::type>;
    };

    template<template<typename> typename Transform, std::size_t ID, typename Name, typename Inner>
    struct traverse<Transform, capture<ID, Name, Inner>>
    {
        using type = capture<ID, Name, typename Transform<Inner>::type>;
    };
}
#endif //MREGEX_AST_TRAVERSAL_HPP