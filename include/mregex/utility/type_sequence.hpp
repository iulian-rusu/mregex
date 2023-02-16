#ifndef MREGEX_UTILITY_TYPE_SEQUENCE_HPP
#define MREGEX_UTILITY_TYPE_SEQUENCE_HPP

#include <type_traits>
#include <mregex/symbols/core.hpp>

namespace meta
{
    /**
     * Metacontainer for a sequence of types.
     */
    template<typename... Elems>
    struct type_sequence {};

    /**
     * Checks if a given type sequence has no elements.
     */
    template<typename Sequence>
    inline constexpr bool is_empty = false;

    template<>
    inline constexpr bool is_empty<type_sequence<>> = true;

    /**
     * Queries the type at the front of the sequence.
     */
    template<typename Sequence>
    struct front;

    template<typename First, typename... Rest>
    struct front<type_sequence<First, Rest ...>>
    {
        using type = First;
    };

    template<>
    struct front<type_sequence<>>
    {
        using type = symbol::empty;
    };

    template<typename Sequence>
    using front_t = typename front<Sequence>::type;

    /**
     * Pushes a type to the front of the type sequence.
     */
    template<typename Sequence, typename T>
    struct push;

    template<typename... Elems, typename T>
    struct push<type_sequence<Elems ...>, T>
    {
        using type = type_sequence<T, Elems ...>;
    };

    template<typename... Elems, typename... Ts>
    struct push<type_sequence<Elems ...>, type_sequence<Ts ...>>
    {
        using type = type_sequence<Ts ..., Elems ...>;
    };

    template<typename Sequence, typename T>
    using push_t = typename push<Sequence, T>::type;

    /**
     * Removes a type from the front of the type sequence.
     */
    template<typename Sequence>
    struct pop;

    template<typename First, typename... Rest>
    struct pop<type_sequence<First, Rest ...>>
    {
        using type = type_sequence<Rest ...>;
    };

    template<>
    struct pop<type_sequence<>>
    {
        using type = type_sequence<>;
    };

    template<typename Sequence>
    using pop_t = typename pop<Sequence>::type;

    /**
     * Concatenates multiple type sequences.
     */
    template<typename... Sequences>
    struct concat;

    template<typename... Sequences>
    using concat_t = typename concat<Sequences ...>::type;

    template<>
    struct concat<>
    {
        using type = type_sequence<>;
    };

    template<typename Sequence>
    struct concat<Sequence>
    {
        using type = Sequence;
    };

    template<typename... T0, typename... T1>
    struct concat<type_sequence<T0 ...>, type_sequence<T1 ...>>
    {
        using type = type_sequence<T0 ..., T1 ...>;
    };

    template<typename... T0, typename... T1, typename... T2>
    struct concat<type_sequence<T0 ...>, type_sequence<T1 ...>, type_sequence<T2 ...>>
    {
        using type = type_sequence<T0 ..., T1 ..., T2 ...>;
    };

    template<typename... T0, typename... T1, typename... T2, typename... T3, typename... Sequences>
    struct concat<type_sequence<T0 ...>, type_sequence<T1 ...>, type_sequence<T2 ...>, type_sequence<T3 ...>, Sequences ...>
    {
        using type = concat_t<type_sequence<T0 ..., T1 ..., T2 ..., T3 ...>, Sequences ...>;
    };

    /**
     * Reverses the order of elements in a template type pack.
     */
    template<typename... Elems>
    struct reverse_type_pack
    {
        using type = type_sequence<Elems ...>;
    };

    template<typename... Elems>
    using reverse_type_pack_t = typename reverse_type_pack<Elems ...>::type;

    template<typename T0, typename T1>
    struct reverse_type_pack<T0, T1>
    {
        using type = type_sequence<T1, T0>;
    };

    template<typename T0, typename T1, typename T2>
    struct reverse_type_pack<T0, T1, T2>
    {
        using type = type_sequence<T2, T1, T0>;
    };

    template<typename T0, typename T1, typename T2, typename T3, typename... Rest>
    struct reverse_type_pack<T0, T1, T2, T3, Rest ...>
    {
        using type = concat_t<reverse_type_pack_t<Rest ...>, type_sequence<T3, T2, T1, T0>>;
    };

    /**
     * Reverses the order of elements in a type sequence.
     */
    template<typename Sequence>
    struct reverse;

    template<typename... Elems>
    struct reverse<type_sequence<Elems ...>>
    {
        using type = reverse_type_pack_t<Elems ...>;
    };

    template<typename Sequence>
    using reverse_t = typename reverse<Sequence>::type;
}
#endif //MREGEX_UTILITY_TYPE_SEQUENCE_HPP