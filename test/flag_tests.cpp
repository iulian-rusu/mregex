#include "tests.hpp"

#ifdef MREGEX_RUN_FLAG_TESTS
namespace meta::tests
{
    static_assert(is_flag_enabled_v<flag::icase, flag::i>);
    static_assert(is_flag_enabled_v<flag::i, flag::icase, flag::U>);
    static_assert(is_flag_enabled_v<flag::multiline, flag::i, flag::m>);
    static_assert(is_flag_enabled_v<flag::m, flag::icase, flag::U, flag::U, flag::multiline>);
    static_assert(is_flag_enabled_v<flag::dotall, flag::s, flag::dotall, flag::dotall>);
    static_assert(is_flag_enabled_v<flag::s, flag::icase, flag::U, flag::s>);
    static_assert(is_flag_enabled_v<flag::ungreedy, flag::ungreedy>);
    static_assert(is_flag_enabled_v<flag::r, flag::icase, flag::unroll, flag::i>);
    static_assert(is_flag_enabled_v<flag::unroll, flag::U, flag::r>);
    static_assert(is_flag_enabled_v<flag::ungreedy, flag::ungreedy>);
    static_assert(is_flag_enabled_v<flag::U, void, flag::icase, flag::ungreedy>);
    static_assert(is_flag_enabled_v<flag::icase, flag::U> == false);
    static_assert(is_flag_enabled_v<flag::i, flag::m, flag::U> == false);
    static_assert(is_flag_enabled_v<flag::multiline> == false);
    static_assert(is_flag_enabled_v<flag::unroll, flag::ungreedy> == false);
    static_assert(is_flag_enabled_v<flag::s, int, void, char, double> == false);
}
#endif //MREGEX_RUN_FLAG_TESTS