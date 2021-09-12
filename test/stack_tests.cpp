#include "tests.hpp"

#ifdef MREGEX_RUN_STACK_TESTS
namespace meta::tests
{
    namespace detail
    {
        template<typename First, typename... Rest>
        struct push_all
        {
            using type = typename push_all<Rest ...>::type::template push<First>;
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
    static_assert(std::is_same_v<stack<>, stack<>::pop>);
    // Push one type to the stack
    static_assert(std::is_same_v<stack<int>, stack<>::push<int>>);
    // Top of (double, int, char) must be double
    static_assert(std::is_same_v<double, stack<double, int, char>::top>);
    // Push three types to the stack
    static_assert(std::is_same_v<stack<double, char, int>, stack<>::push<int>::push<char>::push<double>>);
    // Push three types and pop two
    static_assert(std::is_same_v<stack<int>, stack<>::push<int>::push<char>::push<double>::pop::pop>);
    // Pushing a stack on the stack must yield the concatenation of two stacks
    static_assert(std::is_same_v<stack<double, int, long, char>, stack<long, char>::push<stack<double, int>>>);
    // Pushing multiple elements on the stack
    static_assert(std::is_same_v<stack<char, int, float, double, long, long double, long long, short int>,
            detail::push_all_t<char, int, float, double, long, long double, long long, short int>>);
}
#endif //MREGEX_RUN_STACK_TESTS