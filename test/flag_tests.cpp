#include <mregex.hpp>

namespace meta::tests
{
    static_assert(is_flag_enabled<flag::icase, flag::icase>);
    static_assert(is_flag_enabled<flag::icase, flag::icase, flag::ungreedy>);
    static_assert(is_flag_enabled<flag::icase, flag::multiline, flag::ungreedy, flag::icase>);
    static_assert(is_flag_enabled<flag::icase, flag::multiline, void, flag::icase, flag::ungreedy>);
    static_assert(is_flag_enabled<flag::icase, flag::multiline, void, char, flag::ungreedy, flag::icase>);

    static_assert(is_flag_enabled<flag::icase> == false);
    static_assert(is_flag_enabled<flag::icase, flag::ungreedy> == false);
    static_assert(is_flag_enabled<flag::icase, flag::multiline, flag::ungreedy> == false);
    static_assert(is_flag_enabled<flag::icase, void> == false);
    static_assert(is_flag_enabled<flag::icase, void, char, int, double> == false);
}