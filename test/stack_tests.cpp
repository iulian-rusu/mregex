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
            using type = stack<First>;
        };

        template<typename First, typename... Rest>
        using push_all_t = typename push_all<First, Rest ...>::type;
    }

    // Top of an empty stack must be the type meta::empty_stack
    static_assert(std::is_same_v<empty_stack_marker, stack<>::top>);
    // Popping empty stack should do nothing
    static_assert(std::is_same_v<stack<>, pop<stack<>>>);
    // Push one type to the stack
    static_assert(std::is_same_v<stack<int>, push<stack<>, int>>);
    // Top of (double, int, char) must be double
    static_assert(std::is_same_v<double, stack<double, int, char>::top>);
    // Push three types to the stack
    static_assert(std::is_same_v<stack<double, char, int>, push<push<push<stack<>, int>, char>, double>>);
    // Push three types and pop two
    static_assert(std::is_same_v<stack<int>, pop<pop<push<push<push<stack<>, int>, char>, double>>>>);
    // Pushing a stack on the stack must yield the concatenation of two stacks
    static_assert(std::is_same_v<stack<double, int, long, char>, push<stack<long, char>, stack<double, int>>>);
    // Pushing multiple elements on the stack
    static_assert(std::is_same_v<
        stack<char, int, float, double, long, long double, long long, short int>,
        push_all_t<char, int, float, double, long, long double, long long, short int>
    >);
    // Concatenate multiple stacks
    static_assert(std::is_same_v<
        stack<char, int, float, double, long, long double, long long, short int>,
        concat
        <
            stack<char>,
            stack<int, float>,
            stack<double, long, long double>,
            stack<long long, short int>
        >
    >);
}