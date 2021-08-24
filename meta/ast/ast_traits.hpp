#ifndef META_AST_TRAITS_HPP
#define META_AST_TRAITS_HPP

#include "astfwd.hpp"

namespace meta::ast
{
    /**
     * Type trait to identify nodes that can be trivially matched.
     * A type T is trivially matchable if it always consumes one character when matching.
     * A type T is detected as trivially matchable by checking if it contains a template
     * for a static member function consume_one<A, B>, where A and B can be any generic types.
     */
    template<typename T>
    struct is_trivially_matchable
    {
        template<typename Test>
        static auto sfinae_helper(int) -> decltype(&Test::template consume_one<int, int>, std::true_type{});

        template<typename Test>
        static auto sfinae_helper(...) -> std::false_type;

        static constexpr bool value = decltype(sfinae_helper<T>(int{}))::value;
    };

    template<typename T>
    inline constexpr bool is_trivially_matchable_v = is_trivially_matchable<T>::value;

    template<typename ... Elems>
    inline constexpr bool are_trivially_matchable_v = (is_trivially_matchable_v<Elems> && ...);

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

    template<typename A, typename B, typename Inner>
    struct has_atomic_group<repetition<A, B, Inner>>
    {
        static constexpr bool value = has_atomic_group<Inner>::value;
    };

    template<typename T>
    inline constexpr bool has_atomic_group_v = has_atomic_group<T>::value;
}
#endif //META_AST_TRAITS_HPP