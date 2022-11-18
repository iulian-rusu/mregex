#ifndef MREGEX_UTILITY_TYPE_SEQUENCE_HPP
#define MREGEX_UTILITY_TYPE_SEQUENCE_HPP

#include <mregex/symbols.hpp>

namespace meta
{
    /**
     * Metacontainer for a sequence of types.
     */
    template<typename...>
    struct type_sequence;

    namespace detail
    {
        template<typename, typename>
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

        template<typename>
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

        template<typename... Sequences>
        struct concat
        {
            using type = type_sequence<>;
        };

        template<typename Sequence>
        struct concat<Sequence>
        {
            using type = Sequence;
        };

        template<typename... First, typename... Second>
        struct concat<type_sequence<First ...>, type_sequence<Second ...>>
        {
            using type = type_sequence<First ..., Second ...>;
        };

        template<typename... T0, typename... T1, typename... T2>
        struct concat<type_sequence<T0 ...>, type_sequence<T1 ...>, type_sequence<T2 ...>>
        {
            using type = type_sequence<T0 ..., T1 ..., T2 ...>;
        };

        template<typename... T0, typename... T1, typename... T2, typename... T3, typename... Sequences>
        struct concat<type_sequence<T0 ...>, type_sequence<T1 ...>, type_sequence<T2 ...>, type_sequence<T3 ...>, Sequences ...>
        {
            using type = typename detail::concat<type_sequence<T0 ..., T1 ..., T2 ..., T3 ...>, Sequences ...>::type;
        };

        template<typename... Elems>
        struct reverse_sequence
        {
            using type = type_sequence<Elems ...>;
        };

        template<typename First, typename Second>
        struct reverse_sequence<First, Second>
        {
            using type = type_sequence<Second, First>;
        };

        template<typename T0, typename T1, typename T2>
        struct reverse_sequence<T0, T1, T2>
        {
            using type = type_sequence<T2, T1, T0>;
        };

        template<typename T0, typename T1, typename T2, typename T3, typename... Rest>
        struct reverse_sequence<T0, T1, T2, T3, Rest ...>
        {
            using type = typename concat<typename reverse_sequence<Rest ...>::type, type_sequence<T3, T2, T1, T0>>::type;
        };

        template<typename>
        struct reverse;

        template<typename... Elems>
        struct reverse<type_sequence<Elems ...>>
        {
            using type = typename reverse_sequence<Elems ...>::type;
        };
    }

    template<typename First, typename... Rest>
    struct type_sequence<First, Rest ...>
    {
        using top = First;
    };

    template<>
    struct type_sequence<>
    {
        using top = symbol::empty;
    };

    /**
     * Pushes a type to the front of the sequence.
     */
    template<typename Sequence, typename T>
    using push = typename detail::push<Sequence, T>::type;

    /**
     * Removes a type from the front of the sequence.
     */
    template<typename Sequence>
    using pop = typename detail::pop<Sequence>::type;

    /**
     * Queries the type at the front of the sequence.
     */
    template<typename Sequence>
    using top = typename Sequence::top;

    /**
     * Concatenates multiple sequences.
     */
    template<typename... Sequences>
    using concat = typename detail::concat<Sequences ...>::type;

    /**
     * Reverses the order of elements in a sequence.
     */
    template<typename Sequence>
    using reverse = typename detail::reverse<Sequence>::type;

    /**
     * Checks if a given sequence has no elements.
     */
    template<typename Sequence>
    inline constexpr bool is_empty = std::is_same_v<symbol::empty, typename Sequence::top>;
}
#endif //MREGEX_UTILITY_TYPE_SEQUENCE_HPP