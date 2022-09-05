#include <mregex.hpp>

namespace meta::tests
{
    namespace
    {
        template<typename Sequence, typename First, typename... Rest>
        struct push_sequence
        {
            using type = push<typename push_sequence<Sequence, Rest ...>::type, First>;
        };

        template<typename Sequence, typename Elem>
        struct push_sequence<Sequence, Elem>
        {
            using type = push<Sequence, Elem>;
        };

        template<typename Sequence, typename... Elems>
        using push_sequence_t = typename push_sequence<Sequence, Elems ...>::type;
    }

    static_assert(std::is_same_v<top<type_sequence<>>, symbol::empty>);
    static_assert(std::is_same_v<top<type_sequence<double, int, char>>, double>);
    static_assert(std::is_same_v<pop<type_sequence<>>, type_sequence<>>);
    static_assert(std::is_same_v<push<type_sequence<>, type_sequence<>>, type_sequence<>>);
    static_assert(std::is_same_v<push<type_sequence<>, int>, type_sequence<int>>);
    static_assert(std::is_same_v<push<push<push<type_sequence<>, int>, char>, double>, type_sequence<double, char, int>>);
    static_assert(std::is_same_v<pop<pop<push<push<push<type_sequence<>, int>, char>, double>>>, type_sequence<int>>);
    static_assert(
        std::is_same_v
        <
            push<type_sequence<long, char>, type_sequence<double, int>>,
            type_sequence<double, int, long, char>
        >
    );
    static_assert(
        std::is_same_v
        <
            push_sequence_t<type_sequence<void>, char, int, float, double, long, long double, long long, short int>,
            type_sequence<char, int, float, double, long, long double, long long, short int, void>
        >
    );
    static_assert(
        std::is_same_v
        <
            concat
            <
                type_sequence<char>,
                type_sequence<int, float>,
                type_sequence<double, long, long double>,
                type_sequence<long long, short int>
            >,
            type_sequence<char, int, float, double, long, long double, long long, short int>
        >
    );
    static_assert(
        std::is_same_v
        <
            reverse<type_sequence<>>,
            type_sequence<>
        >
    );
    static_assert(
        std::is_same_v
        <
            reverse<type_sequence<int>>,
            type_sequence<int>
        >
    );
    static_assert(
        std::is_same_v
        <
            reverse<type_sequence<int, double>>,
            type_sequence<double, int>
        >
    );
    static_assert(
        std::is_same_v
        <
            reverse<type_sequence<char, short, int>>,
            type_sequence<int, short, char>
        >
    );
    static_assert(
        std::is_same_v
        <
            reverse<type_sequence<char, short, int, long>>,
            type_sequence<long, int, short, char>
        >
    );
    static_assert(
        std::is_same_v
        <
            reverse<type_sequence<char, short, int, long, float, double, unsigned, void>>,
            type_sequence<void, unsigned, double, float, long, int, short, char>
        >
    );
}