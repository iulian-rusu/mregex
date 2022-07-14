#ifndef MREGEX_CAPTURE_NAME_SPECIFIER_HPP
#define MREGEX_CAPTURE_NAME_SPECIFIER_HPP

#include <mregex/ast/astfwd.hpp>
#include <mregex/utility/stack.hpp>

namespace meta::ast
{
    /**
     * Metafunction that builds a specification for the names of all capturing groups in the AST.
     * The specification lists all capture names according to Preorder AST traversal.
     *
     * @tparam Node     The AST node with the capturing groups
     */
    template<typename Node>
    struct capture_name_spec
    {
        using type = stack<>;
    };

    template<typename Node>
    using capture_name_spec_t = typename capture_name_spec<Node>::type;

    template<template<typename...> typename Wrapper, typename... Inner>
    struct capture_name_spec<Wrapper<Inner ...>>
    {
        using type = concat<capture_name_spec_t<Inner> ...>;
    };

    template<template<bool, typename> typename Wrapper, bool Lazy, typename Inner>
    struct capture_name_spec<Wrapper<Lazy, Inner>>
    {
        using type = capture_name_spec_t<Inner>;
    };

    template<bool Lazy, typename A, typename B, typename Inner>
    struct capture_name_spec<basic_repetition<Lazy, A, B, Inner>>
    {
        using type = capture_name_spec_t<Inner>;
    };

    template<std::size_t ID, typename Name, typename Inner>
    struct capture_name_spec<capture<ID, Name, Inner>>
    {
        using type = push<capture_name_spec_t<Inner>, Name>;
    };
}
#endif //MREGEX_CAPTURE_NAME_SPECIFIER_HPP