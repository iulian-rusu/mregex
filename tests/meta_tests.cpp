#include "tests.h"

#ifdef CX_RUN_UTILITY_TESTS
namespace cx::tests
{
    static_assert(std::is_same_v<int, first_t<int>>);
    static_assert(std::is_same_v<int, first_t<int, double, char, long, float>>);
    static_assert(std::is_same_v<float, last_t<int, double, char, long, float>>);
    static_assert(std::is_same_v<float, last_t<float>>);
    static_assert(is_any_of_v<int, int>);
    static_assert(is_any_of_v<int, float, int>);
    static_assert(is_any_of_v<int, int, long int, double>);
    static_assert(is_any_of_v<int, int, double, long, void, float>);
    static_assert(is_any_of_v<int, double, double, int, void, void>);
    static_assert(is_any_of_v<int, float, double, long, int, void>);
    static_assert(is_any_of_v<int, float, double, long, void> == false);
    static_assert(is_any_of_v<int, float, double, long> == false);
    static_assert(is_any_of_v<int, float, long int> == false);
    static_assert(is_any_of_v<int> == false);
}
#endif //CX_RUN_UTILITY_TESTS
