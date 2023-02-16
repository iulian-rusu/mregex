#include <mregex.hpp>

namespace meta::tests
{
    namespace
    {
        template<typename Sequence, typename First, typename... Rest>
        struct push_type_pack
        {
            using type = typename push_type_pack<push_t<Sequence, First>, Rest ...>::type;
        };

        template<typename Sequence, typename Elem>
        struct push_type_pack<Sequence, Elem>
        {
            using type = push_t<Sequence, Elem>;
        };

        template<typename Sequence, typename... Elems>
        using push_type_pack_t = typename push_type_pack<Sequence, Elems ...>::type;
    }

    static_assert(std::is_same_v<front_t<type_sequence<>>, symbol::empty>);
    static_assert(std::is_same_v<front_t<type_sequence<char, int, double>>, char>);
    static_assert(std::is_same_v<pop_t<type_sequence<>>, type_sequence<>>);
    static_assert(std::is_same_v<pop_t<push_t<type_sequence<>, int>>, type_sequence<>>);
    static_assert(std::is_same_v<push_t<type_sequence<>, type_sequence<>>, type_sequence<>>);
    static_assert(std::is_same_v<push_t<type_sequence<>, int>, type_sequence<int>>);
    static_assert(
        std::is_same_v
        <
            push_t<push_t<push_t<type_sequence<>, double>, int>, char>,
            type_sequence<char, int, double>
        >
    );
    static_assert(
        std::is_same_v
        <
            pop_t<pop_t<push_t<push_t<push_t<type_sequence<>, double>, int>, char>>>,
            type_sequence<double>
        >
    );
    static_assert(
        std::is_same_v
        <
            push_t<type_sequence<short, long>, type_sequence<signed, unsigned>>,
            type_sequence<signed, unsigned, short, long>
        >
    );
    static_assert(
        std::is_same_v
        <
            push_type_pack_t<type_sequence<void>, char, short, long, float, double, unsigned, signed>,
            type_sequence<signed, unsigned, double, float, long, short, char, void>
        >
    );
    static_assert(
        std::is_same_v
        <
            concat_t
            <
                type_sequence<void>,
                type_sequence<short, long>,
                type_sequence<int, float, double>,
                type_sequence<char, signed, unsigned>,
                type_sequence<bool, bool, bool, bool, bool>
            >,
            type_sequence<void, short, long, int, float, double, char, signed, unsigned, bool, bool, bool, bool, bool>
        >
    );
    static_assert(
        std::is_same_v
        <
            reverse_t<type_sequence<>>,
            type_sequence<>
        >
    );
    static_assert(
        std::is_same_v
        <
            reverse_t<type_sequence<int>>,
            type_sequence<int>
        >
    );
    static_assert(
        std::is_same_v
        <
            reverse_t<type_sequence<int, double>>,
            type_sequence<double, int>
        >
    );
    static_assert(
        std::is_same_v
        <
            reverse_t<type_sequence<char, short, long>>,
            type_sequence<long, short, char>
        >
    );
    static_assert(
        std::is_same_v
        <
            reverse_t<type_sequence<char, short, long, double>>,
            type_sequence<double, long, short, char>
        >
    );
    static_assert(
        std::is_same_v
        <
            reverse_t<type_sequence<void, char, short, long, float, double, unsigned, signed>>,
            type_sequence<signed, unsigned, double, float, long, short, char, void>
        >
    );
}