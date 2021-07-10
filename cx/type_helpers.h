#ifndef CX_TYPE_HELPERS_H
#define CX_TYPE_HELPERS_H

/**
 * File with concepts and metafunctions for type management
 */
namespace cx
{
    /**
     * Concept used to constrain the generic type accepted by matching/searching functions
     */
    template<typename T>
    concept string_like = requires(T s, std::size_t index)
    {
        static_cast<char>(s[index]);
        static_cast<std::size_t>(s.length());
        s.substr(index, index);
    };

    /**
     * Concept used to constrain a type used as a value producer for cx::generator
     */
    template<typename T>
    concept producer = requires(T p)
    {
        p();
    };

    /**
     * Meta-pair containing two types
     *
     * @tparam First    The first type in the pair
     * @tparam Second   The second type in the pair
     */
    template<typename First, typename Second>
    struct pair
    {
        using first = First;
        using second = Second;
    };

    template<typename T>
    struct has_atomic_group
    {
        static constexpr bool value = false;
    };

    template<typename Inner>
    struct has_atomic_group<atomic<Inner>>
    {
        static constexpr bool value = true;
    };

    template<template<typename> typename Outer, typename ... Inners>
    struct has_atomic_group<Outer<Inners ...>>
    {
        static constexpr bool value = (has_atomic_group<Inners>::value || ...);
    };

    template<template<auto, typename> typename Outer, auto I, typename ... Inners>
    struct has_atomic_group<Outer<I, Inners ...>>
    {
        static constexpr bool value = (has_atomic_group<Inners>::value || ...);
    };

    template<typename T>
    constexpr bool has_atomic_group_v = has_atomic_group<T>::value;
}
#endif //CX_TYPE_HELPERS_H
