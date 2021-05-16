#ifndef META_ATOMIC_HPP
#define META_ATOMIC_HPP

#include "astfwd.hpp"

namespace meta::ast
{
    /**
     * Traits to identify some AST nodes.
     */
    template<typename T>
    constexpr bool is_terminal_v = std::is_base_of_v<terminal, T>;

    template<typename T>
    struct is_alternation : std::false_type {};

    template<typename First, typename ... Rest>
    struct is_alternation<alternation<First, Rest ...>> : std::true_type {};

    template<typename T>
    constexpr bool is_alternation_v = is_alternation<T>::value;

    template<typename T>
    constexpr bool is_negatable_v = is_terminal_v<T> || is_alternation_v<T>;

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
#endif //META_ATOMIC_HPP