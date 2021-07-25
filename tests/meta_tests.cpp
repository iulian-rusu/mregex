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
    static_assert(is_alternation_v<alternation<int>>);
    static_assert(is_alternation_v<star<int>> == false);
    static_assert(is_alternation_v<alternation<alternation<int>>>);
    static_assert(is_alternation_v<star<alternation<int>>> == false);
    static_assert(is_range_v<range<1, 2>>);
    static_assert(is_range_v<star<int>> == false);
    static_assert(is_range_v<alternation<range<1, 2>>> == false);
    static_assert(is_terminal_v<character<'a'>>);
    static_assert(is_terminal_v<range<'a', 'z'>>);
    static_assert(is_terminal_v<wildcard>);
    static_assert(is_terminal_v<epsilon>);
    static_assert(is_terminal_v<star<character<'a'>>> == false);
    static_assert(is_terminal_v<optional<character<'a'>>> == false);
    static_assert(is_terminal_v<alternation<character<'a'>>> == false);
    static_assert(is_terminal_v<sequence<character<'a'>>> == false);
}
#endif //CX_RUN_UTILITY_TESTS