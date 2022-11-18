#include <mregex.hpp>

namespace meta::tests
{
    static_assert(is_flag_enabled<flag::icase, flag::i>);
    static_assert(is_flag_enabled<flag::i, flag::icase, flag::U>);
    static_assert(is_flag_enabled<flag::multiline, flag::i, flag::m>);
    static_assert(is_flag_enabled<flag::m, flag::icase, flag::U, flag::U, flag::multiline>);
    static_assert(is_flag_enabled<flag::dotall, flag::s, flag::dotall, flag::dotall>);
    static_assert(is_flag_enabled<flag::s, flag::icase, flag::U, flag::s>);
    static_assert(is_flag_enabled<flag::ungreedy, flag::ungreedy>);
    static_assert(is_flag_enabled<flag::r, flag::icase, flag::unroll, flag::i>);
    static_assert(is_flag_enabled<flag::unroll, flag::U, flag::r>);
    static_assert(is_flag_enabled<flag::ungreedy, flag::ungreedy>);
    static_assert(is_flag_enabled<flag::U, void, flag::icase, flag::ungreedy>);
    static_assert(is_flag_enabled<flag::icase, flag::U> == false);
    static_assert(is_flag_enabled<flag::i, flag::m, flag::U> == false);
    static_assert(is_flag_enabled<flag::multiline> == false);
    static_assert(is_flag_enabled<flag::unroll, flag::ungreedy> == false);
    static_assert(is_flag_enabled<flag::s, int, void, char, double> == false);
}