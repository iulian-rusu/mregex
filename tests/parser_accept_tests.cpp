#include "tests.h"

#ifdef CX_RUN_PARSER_TESTS
namespace cx::tests
{
    namespace detail
    {
        template<static_string const pattern>
        static constexpr auto accepted = parser<pattern>::accepted;
    }

    // Test valid inputs
    static_assert(detail::accepted<R"()">);
    static_assert(detail::accepted<R"(a)">);
    static_assert(detail::accepted<R"(\\)">);
    static_assert(detail::accepted<R"(\\a)">);
    static_assert(detail::accepted<R"(\\D)">);
    static_assert(detail::accepted<R"(abcdefg)">);
    static_assert(detail::accepted<R"((c))">);
    static_assert(detail::accepted<R"((c)\1)">);
    static_assert(detail::accepted<R"(c?)">);
    static_assert(detail::accepted<R"(c*)">);
    static_assert(detail::accepted<R"(c+)">);
    static_assert(detail::accepted<R"(c{2})">);
    static_assert(detail::accepted<R"(c{})">);
    static_assert(detail::accepted<R"(c{?})">);
    static_assert(detail::accepted<R"(c{*})">);
    static_assert(detail::accepted<R"(c{+})">);
    static_assert(detail::accepted<R"(c{|})">);
    static_assert(detail::accepted<R"(c{x})">);
    static_assert(detail::accepted<R"(c{0})">);
    static_assert(detail::accepted<R"(c{123})">);
    static_assert(detail::accepted<R"(c\{123})">);
    static_assert(detail::accepted<R"(c\{)">);
    static_assert(detail::accepted<R"(^c+)">);
    static_assert(detail::accepted<R"(^c+$)">);
    static_assert(detail::accepted<R"(one|two)">);
    static_assert(detail::accepted<R"((one|two)?)">);
    static_assert(detail::accepted<R"((one|two)*)">);
    static_assert(detail::accepted<R"((one|two)+)">);
    static_assert(detail::accepted<R"(one+|two)">);
    static_assert(detail::accepted<R"((one|two?)?)">);
    static_assert(detail::accepted<R"((one*|two)*)">);
    static_assert(detail::accepted<R"((one|two|three?)?)">);
    static_assert(detail::accepted<R"((one+|two|three)*)">);
    static_assert(detail::accepted<R"((one|two+|three)+)">);
    static_assert(detail::accepted<R"((\(+)*)">);
    static_assert(detail::accepted<R"((\++)*)">);
    static_assert(detail::accepted<R"((\?+)?)">);
    static_assert(detail::accepted<R"((\|+\))?)">);
    static_assert(detail::accepted<R"(x(?:x))">);
    static_assert(detail::accepted<R"((?:x)x)">);
    static_assert(detail::accepted<R"((?:x)?)">);
    static_assert(detail::accepted<R"((?:x)+)">);
    static_assert(detail::accepted<R"(x+(?:x)?y*)">);
    static_assert(detail::accepted<R"((?:ab|cd)?)">);
    static_assert(detail::accepted<R"((?:abcd)|xy)">);
    static_assert(detail::accepted<R"((?:ab|cd)|xy)">);
    static_assert(detail::accepted<R"((?:(abc){4}))">);
    static_assert(detail::accepted<R"((?:(?:abc)))">);
    static_assert(detail::accepted<R"(12(ab(?:cd)ef)\1gh)">);
    static_assert(detail::accepted<R"((?:(((?:abd))+)x?)|(?:xyz)+)">);
    static_assert(detail::accepted<R"(x*(x?(abc+(?:xyz)?)+x)*x)">);
    static_assert(detail::accepted<R"(x*(x?(abc+(?:xyz)?)+x)*x\1+\2?)">);
    static_assert(detail::accepted<R"(x\*\(x\?(abc\+(?:xyz)\?)\+x\)\*x)">);
    static_assert(detail::accepted<R"([a])">);
    static_assert(detail::accepted<R"([a][b])">);
    static_assert(detail::accepted<R"([])">);
    static_assert(detail::accepted<R"([a]{12}[])">);
    static_assert(detail::accepted<R"([][b])">);
    static_assert(detail::accepted<R"(^[a]$)">);
    static_assert(detail::accepted<R"(])">);
    static_assert(detail::accepted<R"([]])">);
    static_assert(detail::accepted<R"([^a])">);
    static_assert(detail::accepted<R"([^-a])">);
    static_assert(detail::accepted<R"([-a])">);
    static_assert(detail::accepted<R"([a-z])">);
    static_assert(detail::accepted<R"([]-z])">);
    static_assert(detail::accepted<R"([^a-z])">);
    static_assert(detail::accepted<R"(^[^a-z]$)">);
    static_assert(detail::accepted<R"([^-a-z])">);
    static_assert(detail::accepted<R"([- -[])">);
    static_assert(detail::accepted<R"([a-\\w])">);
    static_assert(detail::accepted<R"(f?[a-zA-Z]?a?)">);
    static_assert(detail::accepted<R"(f?[a-z-]?a?)">);
    static_assert(detail::accepted<R"(f?[a-z-A]?a?)">);
    static_assert(detail::accepted<R"(f*[a-zA-Z]*a?)">);
    static_assert(detail::accepted<R"(f+[a-zA-Z]+a+)">);
    static_assert(detail::accepted<R"(f?[a-zA-Z]?a?[xyzt\w0-9]+d?)">);
    static_assert(detail::accepted<R"(ab|cd|[a-zA-Z]|ab{3}|ac)">);
    static_assert(detail::accepted<R"(ab|(cd|[a-zA-Z]|ab)|ac)">);
    static_assert(detail::accepted<R"(^(ab|(cd|[a-zA-Z]|ab)|ac)$)">);
    static_assert(detail::accepted<R"(((a|\\a)|(d|\\d))?)">);
    static_assert(detail::accepted<R"(((a|\\a+)|(d?|\\d))*)">);
    static_assert(detail::accepted<R"(((a|\\a)|(0|1|2|3))?)">);
    static_assert(detail::accepted<R"((((a|\\a)|(0|1|2|3))?)*x?y+abcd|efgh((x|z)?t)+)">);

    // Test invalid inputs
    static_assert(detail::accepted<R"(()"> == false);
    static_assert(detail::accepted<R"())"> == false);
    static_assert(detail::accepted<R"(+)"> == false);
    static_assert(detail::accepted<R"(?)"> == false);
    static_assert(detail::accepted<R"(*)"> == false);
    static_assert(detail::accepted<R"(|)"> == false);
    static_assert(detail::accepted<R"(^?)"> == false);
    static_assert(detail::accepted<R"(c{2x})"> == false);
    static_assert(detail::accepted<R"(c{)"> == false);
    static_assert(detail::accepted<R"(c{123)"> == false);
    static_assert(detail::accepted<R"(c{123\})"> == false);
    static_assert(detail::accepted<R"($?)"> == false);
    static_assert(detail::accepted<R"(^$+)"> == false);
    static_assert(detail::accepted<R"(^$*)"> == false);
    static_assert(detail::accepted<R"(^*$?)"> == false);
    static_assert(detail::accepted<R"(^?$?)"> == false);
    static_assert(detail::accepted<R"((+))"> == false);
    static_assert(detail::accepted<R"((+))"> == false);
    static_assert(detail::accepted<R"((?))"> == false);
    static_assert(detail::accepted<R"((*))"> == false);
    static_assert(detail::accepted<R"((())"> == false);
    static_assert(detail::accepted<R"(()))"> == false);
    static_assert(detail::accepted<R"((?:))"> == false);
    static_assert(detail::accepted<R"(x(?))"> == false);
    static_assert(detail::accepted<R"((?:xx)"> == false);
    static_assert(detail::accepted<R"(?:x)?)"> == false);
    static_assert(detail::accepted<R"((?:)+)"> == false);
    static_assert(detail::accepted<R"(x+(?:?)*)"> == false);
    static_assert(detail::accepted<R"((?:|x))"> == false);
    static_assert(detail::accepted<R"((?:a)|)"> == false);
    static_assert(detail::accepted<R"(|(?:ab))"> == false);
    static_assert(detail::accepted<R"((?x))"> == false);
    static_assert(detail::accepted<R"((?:abc)"> == false);
    static_assert(detail::accepted<R"((?:abc()))"> == false);
    static_assert(detail::accepted<R"(xy?((?:abc)zt\)?)"> == false);
    static_assert(detail::accepted<R"((|))"> == false);
    static_assert(detail::accepted<R"(a|)"> == false);
    static_assert(detail::accepted<R"(|a)"> == false);
    static_assert(detail::accepted<R"(\)"> == false);
    static_assert(detail::accepted<R"([\])"> == false);
    static_assert(detail::accepted<R"([][)"> == false);
    static_assert(detail::accepted<R"([)"> == false);
    static_assert(detail::accepted<R"([a][)"> == false);
    static_assert(detail::accepted<R"([^a-])"> == false);
    static_assert(detail::accepted<R"(?[a-zA-Z]?a?)"> == false);
    static_assert(detail::accepted<R"(ab||[a-zA-Z])"> == false);
    static_assert(detail::accepted<R"(ab|(cd|[a-zA-Z]|)"> == false);
    static_assert(detail::accepted<R"((a|))"> == false);
    static_assert(detail::accepted<R"((a{|))"> == false);
    static_assert(detail::accepted<R"((|a))"> == false);
    static_assert(detail::accepted<R"((+|-))"> == false);
    static_assert(detail::accepted<R"((Inner|a)+))"> == false);
    static_assert(detail::accepted<R"(\((Inner|a)+))"> == false);
    static_assert(detail::accepted<R"(((((((a))))))"> == false);
    static_assert(detail::accepted<R"(((((((a)))))\))"> == false);
    static_assert(detail::accepted<R"(f?[a-zA-]?a?)"> == false);
    static_assert(detail::accepted<R"(f?[a--z-]?a?)"> == false);
    static_assert(detail::accepted<R"(f?[a-z-A]??)"> == false);
    static_assert(detail::accepted<R"(f*[a-zA-Z*a?)"> == false);
    static_assert(detail::accepted<R"(+[a-zA-Z]+a+)"> == false);
    static_assert(detail::accepted<R"(ab|cd|[a-zA-Z]|ab{3|ac)"> == false);
    static_assert(detail::accepted<R"(ab|(cd|[a-zA-Z]|ab\)|ac)"> == false);
    static_assert(detail::accepted<R"(^(ab||(cd|[a-zA-Z]|ab)|ac)$)"> == false);
    static_assert(detail::accepted<R"(((a|\\a)|\(d|\\d))?)"> == false);
    static_assert(detail::accepted<R"(((a|\\a++)|(d?|\\d))*)"> == false);
    static_assert(detail::accepted<R"(((a|\\a)|(0|1|?2|3))?)"> == false);
    static_assert(detail::accepted<R"((((|\\a)|(0|1|2|3))?)*x?y+abcd|efgh((x|z)?t)+)"> == false);
}
#endif // CX_RUN_PARSER_TESTS

