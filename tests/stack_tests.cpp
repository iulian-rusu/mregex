#include "tests.h"

#ifdef CX_RUN_STACK_TESTS
namespace cx::tests
{
    template<typename First, typename ... Rest>
    struct push_all
    {
        using type = typename push_all<Rest ...>::type::template push<First>;
    };

    template<typename First>
    struct push_all<First>
    {
        using type = stack<First>;
    };

    template<typename First, typename ... Rest>
    using push_all_t = typename push_all<First, Rest ...>::type;

    // top of an empty stack must be the type cx::empty_stack
    static_assert(std::is_same_v<empty_stack, stack<>::top>);
    // popping empty stack should do nothing
    static_assert(std::is_same_v<stack<>, stack<>::pop>);
    // push one type to the stack
    static_assert(std::is_same_v<stack<int>, stack<>::push<int>>);
    // top of (double, int, char) must be double
    static_assert(std::is_same_v<double, stack<double, int, char>::top>);
    // push three types to the stack
    static_assert(std::is_same_v<stack<double, char, int>, stack<>::push<int>::push<char>::push<double>>);
    // push three types and pop two
    static_assert(std::is_same_v<stack<int>, stack<>::push<int>::push<char>::push<double>::pop::pop>);
    // pushing a stack on the stack must yield the concatenation of two stacks
    static_assert(std::is_same_v<stack<double, int, long, char>, stack<long, char>::push<stack<double, int>>>);
    // pushing multiple elements on the stack
    static_assert(std::is_same_v<stack<char, int, float, double, long>, push_all_t<char, int, float, double, long>>);
}
#endif //CX_RUN_STACK_TESTS
