#include "tests.h"

#ifdef CTR_RUN_PARSER_TESTS
namespace ctr::tests
{
    // test valid inputs
    static_assert(parser<"">::accepted);
    static_assert(parser<"a">::accepted);
    static_assert(parser<"\\a">::accepted);
    static_assert(parser<"d">::accepted);
    static_assert(parser<"\\d">::accepted);
    static_assert(parser<"abcdefg">::accepted);
    static_assert(parser<"(c)">::accepted);
    static_assert(parser<"c?">::accepted);
    static_assert(parser<"c*">::accepted);
    static_assert(parser<"c+">::accepted);
    static_assert(parser<"one|two">::accepted);
    static_assert(parser<"(one|two)?">::accepted);
    static_assert(parser<"(one|two)*">::accepted);
    static_assert(parser<"(one|two)+">::accepted);
    static_assert(parser<"(\\(+)*">::accepted);
    static_assert(parser<"(\\++)*">::accepted);
    static_assert(parser<"(\\?+)?">::accepted);
    static_assert(parser<"(\\|+\\))?">::accepted);
    static_assert(parser<"\\\\">::accepted);
    static_assert(parser<"((a|\\a)|(d|\\d))?">::accepted);
    static_assert(parser<"((a|\\a+)|(d?|\\d))*">::accepted);
    static_assert(parser<"((a|\\a)|(0|1|2|3))?">::accepted);
    static_assert(parser<"(((a|\\a)|(0|1|2|3))?)*x?y+abcd|efgh((x|z)?t)+">::accepted);
    // test invalid inputs
    static_assert(parser<"(">::accepted == false);
    static_assert(parser<")">::accepted == false);
    static_assert(parser<"+">::accepted == false);
    static_assert(parser<"?">::accepted == false);
    static_assert(parser<"*">::accepted == false);
    static_assert(parser<"|">::accepted == false);
    static_assert(parser<"(+)">::accepted == false);
    static_assert(parser<"(+)">::accepted == false);
    static_assert(parser<"(?)">::accepted == false);
    static_assert(parser<"(*)">::accepted == false);
    static_assert(parser<"(()">::accepted == false);
    static_assert(parser<"())">::accepted == false);
    static_assert(parser<"(|)">::accepted == false);
    static_assert(parser<"a|">::accepted == false);
    static_assert(parser<"|a">::accepted == false);
    static_assert(parser<"\\">::accepted == false);
    static_assert(parser<"(a|)">::accepted == false);
    static_assert(parser<"(|a)">::accepted == false);
    static_assert(parser<"(+|-)">::accepted == false);
    static_assert(parser<"(A|a)+)">::accepted == false);
    static_assert(parser<"\\((A|a)+)">::accepted == false);
    static_assert(parser<"((((((a)))))">::accepted == false);
    static_assert(parser<"((((((a)))))\\)">::accepted == false);
}
#endif // CTR_RUN_PARSER_TESTS

