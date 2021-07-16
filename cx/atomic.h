#ifndef CX_ATOMIC_H
#define CX_ATOMIC_H

namespace cx
{
    template<typename>
    struct atomic;

    /**
    * Helper type trait to find if the tree-like template structure
    * contains at least one atomic group
    */
    template<typename T>
    struct has_atomic_group : std::false_type {};

    template<typename Inner>
    struct has_atomic_group<atomic<Inner>> : std::true_type {};

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
#endif //CX_ATOMIC_H
