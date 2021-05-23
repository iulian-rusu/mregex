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
    static_assert(accepted<"[a]">);
    static_assert(accepted<"]">);
    static_assert(accepted<"[]]">);
    static_assert(accepted<"[^a]">);
    static_assert(accepted<"[^-a]">);
    static_assert(accepted<"[-a]">);
    static_assert(accepted<"[a-z]">);
    static_assert(accepted<"[]-z]">);
    static_assert(accepted<"[^a-z]">);
    static_assert(accepted<"[^-a-z]">);
    static_assert(accepted<"[- -[]">);
    static_assert(accepted<"[a-\\w]">);
    static_assert(accepted<"f?[a-zA-Z]?a?">);
    static_assert(accepted<"f*[a-zA-Z]*a?">);
    static_assert(accepted<"f+[a-zA-Z]+a+">);
    static_assert(accepted<"ab|cd|[a-zA-Z]|ab|ac">);
    static_assert(accepted<"ab|(cd|[a-zA-Z]|ab)|ac">);
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
    static_assert(accepted<"[]"> == false);
    static_assert(accepted<"["> == false);
    static_assert(accepted<"[^a-]"> == false);
    static_assert(accepted<"?[a-zA-Z]?a?"> == false);
    static_assert(accepted<"ab||[a-zA-Z]"> == false);
    static_assert(accepted<"ab|(cd|[a-zA-Z]|"> == false);
    static_assert(accepted<"(a|)"> == false);
    static_assert(accepted<"(|a)"> == false);
    static_assert(accepted<"(+|-)"> == false);
    static_assert(accepted<"(Inner|a)+)"> == false);
    static_assert(accepted<"\\((Inner|a)+)"> == false);
    static_assert(accepted<"((((((a)))))"> == false);
    static_assert(accepted<"((((((a)))))\\)"> == false);
}
#endif // CX_RUN_PARSER_TESTS

