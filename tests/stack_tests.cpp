#include "tests.h"

#ifdef CTR_RUN_STACK_TESTS
namespace ctr::tests
{
    // top of empty stack must be the type ctr::empty_stack
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
}
#endif //CTR_RUN_STACK_TESTS
