#include "tests.h"

#ifdef CX_RUN_FLAG_TESTS
namespace cx::tests
{
    static_assert(std::is_same_v<alias<flag::ignore_case>, flag::i>);
    static_assert(std::is_same_v<alias<flag::i>, flag::ignore_case>);
    static_assert(std::is_same_v<alias<flag::multiline>, flag::m>);
    static_assert(std::is_same_v<alias<flag::m>, flag::multiline>);
    static_assert(std::is_same_v<alias<flag::dotall>, flag::s>);
    static_assert(std::is_same_v<alias<flag::s>, flag::dotall>);
    static_assert(std::is_same_v<alias<flag::extended>, flag::x>);
    static_assert(std::is_same_v<alias<flag::x>, flag::extended>);
    static_assert(std::is_same_v<alias<flag::greedy_alt>, flag::a>);
    static_assert(std::is_same_v<alias<flag::a>, flag::greedy_alt>);
}
#endif //CX_RUN_FLAG_TESTS