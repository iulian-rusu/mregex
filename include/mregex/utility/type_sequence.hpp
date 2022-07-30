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
        struct push;

        template<typename... Elems, typename NewElem>
        struct push<type_sequence<Elems ...>, NewElem>
        {
            using type = type_sequence<NewElem, Elems ...>;
        };

        template<typename... Elems, typename... NewElems>
        struct push<type_sequence<Elems ...>, type_sequence<NewElems ...>>
        {
            using type = type_sequence<NewElems ..., Elems ...>;
        };

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

        template<typename... First, typename... Second, typename... Third>
        struct concat<type_sequence<First ...>, type_sequence<Second ...>, type_sequence<Third ...>>
        {
            using type = type_sequence<First ..., Second ..., Third ...>;
        };

        template<typename... As, typename... Bs, typename... Cs, typename... Ds, typename... Sequences>
        struct concat<type_sequence<As ...>, type_sequence<Bs ...>, type_sequence<Cs ...>, type_sequence<Ds ...>, Sequences ...>
        {
            using type = typename detail::concat<type_sequence<As ..., Bs ..., Cs ..., Ds ...>, Sequences ...>::type;
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
    using push = typename detail::push<Sequence, T>::type;

    template<typename Sequence>
    using pop = typename detail::pop<Sequence>::type;

    template<typename Sequence>
    using top = typename Sequence::top;

    template<typename... Sequences>
    using concat = typename detail::concat<Sequences ...>::type;

    template<typename Sequence>
    inline constexpr bool is_empty_v = std::is_same_v<symbol::empty, typename Sequence::top>;
}
#endif //MREGEX_UTILITY_TYPE_SEQUENCE_HPP