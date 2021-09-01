#include "tests.hpp"

#ifdef META_RUN_FLAG_TESTS
namespace meta::tests
{
    static_assert(std::is_same_v<alias<flag::ignore_case>, flag::i>);
    static_assert(std::is_same_v<alias<flag::i>, flag::ignore_case>);
    static_assert(std::is_same_v<alias<flag::multiline>, flag::m>);
    static_assert(std::is_same_v<alias<flag::m>, flag::multiline>);
    static_assert(std::is_same_v<alias<flag::dotall>, flag::s>);
    static_assert(std::is_same_v<alias<flag::s>, flag::dotall>);
    static_assert(std::is_same_v<alias<flag::ungreedy>, flag::U>);
    static_assert(std::is_same_v<alias<flag::U>, flag::ungreedy>);
}
#endif //META_RUN_FLAG_TESTS