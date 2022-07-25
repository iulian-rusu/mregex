#ifndef MREGEX_UTILITY_TYPE_SEQUENCE_HPP
#define MREGEX_UTILITY_TYPE_SEQUENCE_HPP

#include <mregex/symbols.hpp>

namespace meta
{
    /*
     * Metacontainer for a sequence of types.
     */
    template<typename...>
    struct type_sequence;

    namespace detail
    {
        template<typename Sequence, typename T>
        struct push_impl;

        template<typename... Elems, typename T>
        struct push_impl<type_sequence<Elems ...>, T>
        {
            using type = type_sequence<T, Elems ...>;
        };

        template<typename... Elems, typename... Ts>
        struct push_impl<type_sequence<Elems ...>, type_sequence<Ts ...>>
        {
            using type = type_sequence<Ts ..., Elems ...>;
        };

        template<typename Sequence>
        struct pop_impl;

        template<typename First, typename... Rest>
        struct pop_impl<type_sequence<First, Rest ...>>
        {
            using type = type_sequence<Rest ...>;
        };

        template<>
        struct pop_impl<type_sequence<>>
        {
            using type = type_sequence<>;
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
     * Metafunctions and operators for working with the container
     */

    template<typename Sequence, typename T>
    using push = typename detail::push_impl<Sequence, T>::type;

    template<typename Sequence>
    using pop = typename detail::pop_impl<Sequence>::type;

    template<typename Sequence>
    using top = typename Sequence::top;

    template<typename T, typename... Ts>
    constexpr auto operator<<(type_sequence<Ts ...>, T) -> push<type_sequence<Ts ...>, T> { return {}; }

    template<typename T, typename... Ts>
    constexpr auto operator>>(T, type_sequence<Ts ...>) -> push<type_sequence<Ts ...>, T> { return {}; }

    template<typename... Sequences>
    using concat = decltype((Sequences{} >> ...));

    template<typename Sequence>
    inline constexpr bool is_empty_v = std::is_same_v<symbol::empty, typename Sequence::top>;
}
#endif //MREGEX_UTILITY_TYPE_SEQUENCE_HPP