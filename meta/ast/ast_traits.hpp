#ifndef META_AST_TRAITS_HPP
#define META_AST_TRAITS_HPP

#include "astfwd.hpp"

namespace meta::ast
{
    /**
     * Type trait to identify nodes that can be trivially matched.
     * A node is trivially matchable if it always consumes one character while matching.
     * Being trivially matchabale is signaled by defining a static function template
     * consume_one<T, U> and is checked using SFINAE tricks.
     */
    template<typename T>
    struct is_trivially_matchable
    {
        template<typename Test>
        static auto sfinae_helper(int) -> decltype(Test::template consume_one<int, int>, std::true_type{});

        template<typename Test>
        static auto sfinae_helper(...) -> std::false_type;

        static constexpr bool value = decltype(sfinae_helper<T>(int{}))::value;
    };

    template<typename T>
    constexpr bool is_trivially_matchable_v = is_trivially_matchable<T>::value;

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