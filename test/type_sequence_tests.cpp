#include <mregex.hpp>

namespace meta::tests
{
    namespace
    {
        template<typename First, typename... Rest>
        struct push_all
        {
            using type = push<typename push_all<Rest ...>::type, First>;
        };

        template<typename First>
        struct push_all<First>
        {
            using type = type_sequence<First>;
        };

        template<typename First, typename... Rest>
        using push_all_t = typename push_all<First, Rest ...>::type;
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
            push_all_t<char, int, float, double, long, long double, long long, short int>,
            type_sequence<char, int, float, double, long, long double, long long, short int>
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