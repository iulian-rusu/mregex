#include "tests.h"

#ifdef CTR_RUN_UTILITY_TESTS
namespace ctr::tests
{
    static_assert(std::is_same_v<int, first_t<int>>);
    static_assert(std::is_same_v<int, first_t<int, double, char, long, float>>);
    static_assert(std::is_same_v<float, last_t<int, double, char, long, float>>);
    static_assert(std::is_same_v<float, last_t<float>>);
}
#endif //CTR_RUN_UTILITY_TESTS
