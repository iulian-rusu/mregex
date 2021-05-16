#include "tests.h"

#ifdef CX_RUN_PARSER_TESTS
namespace cx::tests
{
    template<static_string const pattern>
    static constexpr auto accepted = parser<pattern>::accepted;

    // test valid inputs
    static_assert(accepted<"">);
    static_assert(accepted<"a">);
    static_assert(accepted<"\\a">);
    static_assert(accepted<"\\D">);
    static_assert(accepted<"abcdefg">);
    static_assert(accepted<"(c)">);
    static_assert(accepted<"c?">);
    static_assert(accepted<"c*">);
    static_assert(accepted<"c+">);
    static_assert(accepted<"one|two">);
    static_assert(accepted<"(one|two)?">);
    static_assert(accepted<"(one|two)*">);
    static_assert(accepted<"(one|two)+">);
    static_assert(accepted<"(\\(+)*">);
    static_assert(accepted<"(\\++)*">);
    static_assert(accepted<"(\\?+)?">);
    static_assert(accepted<"(\\|+\\))?">);
    static_assert(accepted<"\\\\">);
    static_assert(accepted<"((a|\\a)|(d|\\d))?">);
    static_assert(accepted<"((a|\\a+)|(d?|\\d))*">);
    static_assert(accepted<"((a|\\a)|(0|1|2|3))?">);
    static_assert(accepted<"(((a|\\a)|(0|1|2|3))?)*x?y+abcd|efgh((x|z)?t)+">);
    // test invalid inputs
    static_assert(accepted<"("> == false);
    static_assert(accepted<")"> == false);
    static_assert(accepted<"+"> == false);
    static_assert(accepted<"?"> == false);
    static_assert(accepted<"*"> == false);
    static_assert(accepted<"|"> == false);
    static_assert(accepted<"(+)"> == false);
    static_assert(accepted<"(+)"> == false);
    static_assert(accepted<"(?)"> == false);
    static_assert(accepted<"(*)"> == false);
    static_assert(accepted<"(()"> == false);
    static_assert(accepted<"())"> == false);
    static_assert(accepted<"(|)"> == false);
    static_assert(accepted<"a|"> == false);
    static_assert(accepted<"|a"> == false);
    static_assert(accepted<"\\"> == false);
    static_assert(accepted<"(a|)"> == false);
    static_assert(accepted<"(|a)"> == false);
    static_assert(accepted<"(+|-)"> == false);
    static_assert(accepted<"(First|a)+)"> == false);
    static_assert(accepted<"\\((First|a)+)"> == false);
    static_assert(accepted<"((((((a)))))"> == false);
    static_assert(accepted<"((((((a)))))\\)"> == false);
}
#endif // CX_RUN_PARSER_TESTS

