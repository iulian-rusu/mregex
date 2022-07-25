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

    static_assert(std::is_same_v<symbol::empty, top<type_sequence<>>>);
    static_assert(std::is_same_v<type_sequence<>, pop<type_sequence<>>>);
    static_assert(std::is_same_v<type_sequence<>, push<type_sequence<>, type_sequence<>>>);
    static_assert(std::is_same_v<type_sequence<int>, push<type_sequence<>, int>>);
    static_assert(std::is_same_v<double, type_sequence<double, int, char>::top>);
    static_assert(std::is_same_v<type_sequence<double, char, int>, push<push<push<type_sequence<>, int>, char>, double>>);
    static_assert(std::is_same_v<type_sequence<int>, pop<pop<push<push<push<type_sequence<>, int>, char>, double>>>>);
    static_assert(
        std::is_same_v
        <
            type_sequence<double, int, long, char>,
            push<type_sequence<long, char>, type_sequence<double, int>>
        >
    );
    static_assert(
        std::is_same_v
        <
            type_sequence<char, int, float, double, long, long double, long long, short int>,
            push_all_t<char, int, float, double, long, long double, long long, short int>
        >
    );
    static_assert(
        std::is_same_v
        <
            type_sequence<char, int, float, double, long, long double, long long, short int>,
            concat
            <
                type_sequence<char>,
                type_sequence<int, float>,
                type_sequence<double, long, long double>,
                type_sequence<long long, short int>
            >
        >
    );
}