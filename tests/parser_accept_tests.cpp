#include "tests.h"

#ifdef CX_RUN_PARSER_TESTS
namespace cx::tests
{
    template<static_string const pattern>
    static constexpr auto accepted = parser<pattern>::accepted;

    // Test valid inputs
    static_assert(accepted<R"()">);
    static_assert(accepted<R"(a)">);
    static_assert(accepted<R"(\\a)">);
    static_assert(accepted<R"(\\D)">);
    static_assert(accepted<R"(abcdefg)">);
    static_assert(accepted<R"((c))">);
    static_assert(accepted<R"((c)\1)">);
    static_assert(accepted<R"(c?)">);
    static_assert(accepted<R"(c*)">);
    static_assert(accepted<R"(c+)">);
    static_assert(accepted<R"(^c+)">);
    static_assert(accepted<R"(^c+$)">);
    static_assert(accepted<R"(one|two)">);
    static_assert(accepted<R"((one|two)?)">);
    static_assert(accepted<R"((one|two)*)">);
    static_assert(accepted<R"((one|two)+)">);
    static_assert(accepted<R"((\(+)*)">);
    static_assert(accepted<R"((\++)*)">);
    static_assert(accepted<R"((\?+)?)">);
    static_assert(accepted<R"((\|+\))?)">);
    static_assert(accepted<R"((?:(abc)))">);
    static_assert(accepted<R"((?:(?:abc)))">);
    static_assert(accepted<R"((?:(((?:abd))+)x?)|(?:xyz)+)">);
    static_assert(accepted<R"(x*(x?(abc+(?:xyz)?)+x)*x)">);
    static_assert(accepted<R"(\\)">);
    static_assert(accepted<R"([a])">);
    static_assert(accepted<R"(^[a]$)">);
    static_assert(accepted<R"(])">);
    static_assert(accepted<R"([]])">);
    static_assert(accepted<R"([^a])">);
    static_assert(accepted<R"([^-a])">);
    static_assert(accepted<R"([-a])">);
    static_assert(accepted<R"([a-z])">);
    static_assert(accepted<R"([]-z])">);
    static_assert(accepted<R"([^a-z])">);
    static_assert(accepted<R"(^[^a-z]$)">);
    static_assert(accepted<R"([^-a-z])">);
    static_assert(accepted<R"([- -[])">);
    static_assert(accepted<R"([a-\\w])">);
    static_assert(accepted<R"(f?[a-zA-Z]?a?)">);
    static_assert(accepted<R"(f?[a-z-]?a?)">);
    static_assert(accepted<R"(f?[a-z-A]?a?)">);
    static_assert(accepted<R"(f*[a-zA-Z]*a?)">);
    static_assert(accepted<R"(f+[a-zA-Z]+a+)">);
    static_assert(accepted<R"(ab|cd|[a-zA-Z]|ab|ac)">);
    static_assert(accepted<R"(ab|(cd|[a-zA-Z]|ab)|ac)">);
    static_assert(accepted<R"(^(ab|(cd|[a-zA-Z]|ab)|ac)$)">);
    static_assert(accepted<R"(((a|\\a)|(d|\\d))?)">);
    static_assert(accepted<R"(((a|\\a+)|(d?|\\d))*)">);
    static_assert(accepted<R"(((a|\\a)|(0|1|2|3))?)">);
    static_assert(accepted<R"((((a|\\a)|(0|1|2|3))?)*x?y+abcd|efgh((x|z)?t)+)">);
    // Test invalid inputs
    static_assert(accepted<R"(()"> == false);
    static_assert(accepted<R"())"> == false);
    static_assert(accepted<R"(+)"> == false);
    static_assert(accepted<R"(?)"> == false);
    static_assert(accepted<R"(*)"> == false);
    static_assert(accepted<R"(|)"> == false);
    static_assert(accepted<R"(^?)"> == false);
    static_assert(accepted<R"($?)"> == false);
    static_assert(accepted<R"(^$+)"> == false);
    static_assert(accepted<R"(^$*)"> == false);
    static_assert(accepted<R"(^*$?)"> == false);
    static_assert(accepted<R"(^?$?)"> == false);
    static_assert(accepted<R"((+))"> == false);
    static_assert(accepted<R"((+))"> == false);
    static_assert(accepted<R"((?))"> == false);
    static_assert(accepted<R"((*))"> == false);
    static_assert(accepted<R"((())"> == false);
    static_assert(accepted<R"(()))"> == false);
    static_assert(accepted<R"((?:))"> == false);
    static_assert(accepted<R"((?x))"> == false);
    static_assert(accepted<R"((?:abc)"> == false);
    static_assert(accepted<R"((?:abc()))"> == false);
    static_assert(accepted<R"((|))"> == false);
    static_assert(accepted<R"(a|)"> == false);
    static_assert(accepted<R"(|a)"> == false);
    static_assert(accepted<R"(\)"> == false);
    static_assert(accepted<R"([])"> == false);
    static_assert(accepted<R"([)"> == false);
    static_assert(accepted<R"([^a-])"> == false);
    static_assert(accepted<R"(?[a-zA-Z]?a?)"> == false);
    static_assert(accepted<R"(ab||[a-zA-Z])"> == false);
    static_assert(accepted<R"(ab|(cd|[a-zA-Z]|)"> == false);
    static_assert(accepted<R"((a|))"> == false);
    static_assert(accepted<R"((|a))"> == false);
    static_assert(accepted<R"((+|-))"> == false);
    static_assert(accepted<R"((Inner|a)+))"> == false);
    static_assert(accepted<R"(\((Inner|a)+))"> == false);
    static_assert(accepted<R"(((((((a))))))"> == false);
    static_assert(accepted<R"(((((((a)))))\))"> == false);
}
#endif // CX_RUN_PARSER_TESTS

