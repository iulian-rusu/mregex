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

    // Top of an empty sequence must be the type symbol::empty
    static_assert(std::is_same_v<symbol::empty, top<type_sequence<>>>);
    // Popping empty sequence should do nothing
    static_assert(std::is_same_v<type_sequence<>, pop<type_sequence<>>>);
    // Push one type to the sequence
    static_assert(std::is_same_v<type_sequence<int>, push<type_sequence<>, int>>);
    // Top of (double, int, char) must be double
    static_assert(std::is_same_v<double, type_sequence<double, int, char>::top>);
    // Push three types to the sequence
    static_assert(std::is_same_v<type_sequence<double, char, int>, push<push<push<type_sequence<>, int>, char>, double>>);
    // Push three types and pop two
    static_assert(std::is_same_v<type_sequence<int>, pop<pop<push<push<push<type_sequence<>, int>, char>, double>>>>);
    // Pushing a sequence must be equivalent to concatenation
    static_assert(std::is_same_v<
            type_sequence<double, int, long, char>,
            push<type_sequence<long, char>, type_sequence<double, int>>
    >);
    // Pushing multiple elements to the sequence
    static_assert(std::is_same_v<
        type_sequence<char, int, float, double, long, long double, long long, short int>,
        push_all_t<char, int, float, double, long, long double, long long, short int>
    >);
    // Concatenating multiple sequences
    static_assert(std::is_same_v<
        type_sequence<char, int, float, double, long, long double, long long, short int>,
        concat
        <
            type_sequence<char>,
            type_sequence<int, float>,
            type_sequence<double, long, long double>,
            type_sequence<long long, short int>
        >
    >);
}