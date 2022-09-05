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

    template<template<typename> typename Transform, match_mode Mode, typename A, typename B, typename Inner>
    struct traverse<Transform, basic_repetition<Mode, A, B, Inner>>
    {
        using type = basic_repetition<Mode, A, B, typename Transform<Inner>::type>;
    };

    template<template<typename> typename Transform, std::size_t I, typename Name, typename Inner>
    struct traverse<Transform, capture<I, Name, Inner>>
    {
        using type = capture<I, Name, typename Transform<Inner>::type>;
    };
}
#endif //MREGEX_AST_TRAVERSAL_HPP