#ifndef META_AST_TRAITS_HPP
#define META_AST_TRAITS_HPP

#include "astfwd.hpp"

namespace meta::ast
{
    template<typename T>
    constexpr bool is_terminal_v = std::is_base_of_v<terminal, T>;

    /**
     * Type trait to identify nodes that always consume one chaaacter, used for optimizations
     */
    template<typename T>
    struct consumes_one_on_match : std::false_type {};

    template<typename Inner>
    struct consumes_one_on_match<negated<Inner>> : consumes_one_on_match<Inner> {};

    template<>
    struct consumes_one_on_match<nothing> : std::true_type {};

    template<typename First, typename ... Rest>
    struct consumes_one_on_match<set<First, Rest ...>> : std::true_type {};

    template<auto C>
    struct consumes_one_on_match<character<C>> : std::true_type {};

    template<>
    struct consumes_one_on_match<whitespace> : std::true_type {};

    template<>
    struct consumes_one_on_match<wildcard> : std::true_type {};

    template<auto A, auto B>
    struct consumes_one_on_match<range<A, B>> : std::true_type {};

    template<typename T>
    constexpr bool consumes_one_on_match_v = consumes_one_on_match<T>::value;

    /**
     * Helper type trait to find if the tree-like template structure
     * contains at least one atomic group.
     */
    template<typename T>
    struct has_atomic_group : std::false_type {};

    template<auto ID, typename Inner>
    struct has_atomic_group<atomic<ID, Inner>> : std::true_type {};

    template<template<typename> typename Outer, typename ... Inners>
    struct has_atomic_group<Outer<Inners ...>>
    {
        static constexpr bool value = (has_atomic_group<Inners>::value || ...);
    };

    template<template<auto, typename ...> typename Outer, auto I, typename ... Inners>
    struct has_atomic_group<Outer<I, Inners ...>>
    {
        static constexpr bool value = (has_atomic_group<Inners>::value || ...);
    };

    template<typename T>
    constexpr bool has_atomic_group_v = has_atomic_group<T>::value;
}
#endif //META_AST_TRAITS_HPP