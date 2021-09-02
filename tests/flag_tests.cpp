#include "tests.hpp"

#ifdef META_RUN_FLAG_TESTS
namespace meta::tests
{
    static_assert(std::is_same_v<flag::icase, flag::i>);
    static_assert(std::is_same_v<flag::i, flag::icase>);
    static_assert(std::is_same_v<flag::multiline, flag::m>);
    static_assert(std::is_same_v<flag::m, flag::multiline>);
    static_assert(std::is_same_v<flag::dotall, flag::s>);
    static_assert(std::is_same_v<flag::s, flag::dotall>);
    static_assert(std::is_same_v<flag::ungreedy, flag::U>);
    static_assert(std::is_same_v<flag::U, flag::ungreedy>);
}
#endif //META_RUN_FLAG_TESTS