#include <mregex.hpp>

namespace meta::tests
{
    namespace
    {
        template<static_string Pattern>
        inline constexpr bool accepted = parser<Pattern>::accepted;
    }

    /**
     * Test valid inputs
     */

     // Basic patterns
    static_assert(accepted<R"()">);
    static_assert(accepted<R"(())">);
    static_assert(accepted<R"(a)">);
    static_assert(accepted<R"(abcd)">);
    // Anchors
    static_assert(accepted<R"((^)?)">);
    static_assert(accepted<R"(($)?)">);
    static_assert(accepted<R"((^$)*?)">);
    static_assert(accepted<R"((\b)+)">);
    static_assert(accepted<R"((\B)?+)">);
    static_assert(accepted<R"((\b)+?)">);
    static_assert(accepted<R"(\b\{x1})">);
    static_assert(accepted<R"(\B\{+}?)">);
    static_assert(accepted<R"(\A\{??)">);
    static_assert(accepted<R"(\Z\{ x{3}?x{13,}+)">);
    // Escaped characters
    static_assert(accepted<R"(\\)">);
    static_assert(accepted<R"(\\a)">);
    static_assert(accepted<R"(\\D)">);
    static_assert(accepted<R"(\n)">);
    static_assert(accepted<R"(\N)">);
    static_assert(accepted<R"(\b)">);
    static_assert(accepted<R"(\B)">);
    static_assert(accepted<R"((\(+)*)">);
    static_assert(accepted<R"((\++?)*)">);
    static_assert(accepted<R"((\?+)?)">);
    static_assert(accepted<R"((\?+?)?)">);
    static_assert(accepted<R"((\|+\))?)">);
    static_assert(accepted<R"(x\*\(x\?(abc\+(?:xyz)\?)\+x\)\*x)">);
    // Optional, Kleen star and plus
    static_assert(accepted<R"(c?)">);
    static_assert(accepted<R"(c*)">);
    static_assert(accepted<R"(c+)">);
    static_assert(accepted<R"(c??)">);
    static_assert(accepted<R"(c*?)">);
    static_assert(accepted<R"(c+?)">);
    static_assert(accepted<R"(c?+)">);
    static_assert(accepted<R"(c*+)">);
    static_assert(accepted<R"(c++)">);
    static_assert(accepted<R"(^c+)">);
    static_assert(accepted<R"(^c+$)">);
    // Repetitions
    static_assert(accepted<R"(c{2})">);
    static_assert(accepted<R"(c{2}?)">);
    static_assert(accepted<R"(c{2}+)">);
    static_assert(accepted<R"(c{,0})">);
    static_assert(accepted<R"(c{,0}?)">);
    static_assert(accepted<R"(c{0})">);
    static_assert(accepted<R"(c{123})">);
    static_assert(accepted<R"(c{1,23})">);
    static_assert(accepted<R"(c{12,})">);
    static_assert(accepted<R"(c{0,123})">);
    static_assert(accepted<R"(c{0,123}?)">);
    static_assert(accepted<R"(c{1,2})">);
    static_assert(accepted<R"(c{})">);
    static_assert(accepted<R"(\{})">);
    static_assert(accepted<R"(\{1x})">);
    static_assert(accepted<R"(\{123}x)">);
    static_assert(accepted<R"(\{123,124}x)">);
    static_assert(accepted<R"(c{}?)">);
    static_assert(accepted<R"(c{)">);
    static_assert(accepted<R"(c{?)">);
    static_assert(accepted<R"(c{?})">);
    static_assert(accepted<R"(c{?}?)">);
    static_assert(accepted<R"(c{?}+)">);
    static_assert(accepted<R"(c{*})">);
    static_assert(accepted<R"(c{+})">);
    static_assert(accepted<R"(c{|})">);
    static_assert(accepted<R"(c{x})">);
    static_assert(accepted<R"(c{,})">);
    static_assert(accepted<R"(c{ 0, 123})">);
    static_assert(accepted<R"(c\{123})">);
    static_assert(accepted<R"(c\{)">);
    static_assert(accepted<R"(c*+\{)">);
    // Alternations
    static_assert(accepted<R"(one|two)">);
    static_assert(accepted<R"((one|two)?)">);
    static_assert(accepted<R"((one|two)*)">);
    static_assert(accepted<R"((one|two)+)">);
    static_assert(accepted<R"(one+|two)">);
    static_assert(accepted<R"((one|two?)?)">);
    static_assert(accepted<R"((one*|two)*)">);
    static_assert(accepted<R"((one|two|three?)?)">);
    static_assert(accepted<R"((one+|two|three)*)">);
    static_assert(accepted<R"((one|two+|three)+)">);
    // Groups and backreferences
    static_assert(accepted<R"((c))">);
    static_assert(accepted<R"((c)\1)">);
    static_assert(accepted<R"(x(?:x))">);
    static_assert(accepted<R"((?:x)x)">);
    static_assert(accepted<R"((?:x)?)">);
    static_assert(accepted<R"((?:x)+)">);
    static_assert(accepted<R"((?:))">);
    static_assert(accepted<R"((?:abc()))">);
    static_assert(accepted<R"((?:)+)">);
    static_assert(accepted<R"(x+(?:x)?y*)">);
    static_assert(accepted<R"((?:ab|cd)?)">);
    static_assert(accepted<R"((?:abcd)|xy)">);
    static_assert(accepted<R"((?:ab|cd)|xy)">);
    static_assert(accepted<R"((?:(abc){4}))">);
    static_assert(accepted<R"((?:(?:abc)))">);
    static_assert(accepted<R"(12(ab(?:cd)ef)\1gh)">);
    static_assert(accepted<R"((?:(((?:abd))+)x?)|(?:xyz)+)">);
    static_assert(accepted<R"(x*(x?(abc+(?:xyz)?)+x)*x)">);
    static_assert(accepted<R"(x*(x?(abc+(?:xyz)?)+x)*x\1+\2?)">);
    static_assert(accepted<R"(x*?(x?(abc+?(?:xyz)?)+x)*x\1+?\2??)">);
    static_assert(accepted<R"(x*+(x?(abc+?(?:xyz)?)+x)*x\1++\2?+)">);
   // Sets
    static_assert(accepted<R"([a])">);
    static_assert(accepted<R"([])">);
    static_assert(accepted<R"([\b])">);
    static_assert(accepted<R"([\1])">);
    static_assert(accepted<R"([\0])">);
    static_assert(accepted<R"([\k])">);
    static_assert(accepted<R"([\k<test>])">);
    static_assert(accepted<R"([a][b])">);
    static_assert(accepted<R"([-a])">);
    static_assert(accepted<R"([a-z])">);
    static_assert(accepted<R"([\w-a])">);
    static_assert(accepted<R"([A-\\w])">);
    static_assert(accepted<R"([A-\\D])">);
    static_assert(accepted<R"([^a])">);
    static_assert(accepted<R"([^-a])">);
    static_assert(accepted<R"([^a-])">);
    static_assert(accepted<R"([^])">);
    static_assert(accepted<R"([^a-z])">);
    static_assert(accepted<R"(^[a]$)">);
    static_assert(accepted<R"(^[^a-z]$)">);
    static_assert(accepted<R"([^-a-z])">);
    static_assert(accepted<R"([\0-\n])">);
    static_assert(accepted<R"(f?[a-zA-Z]?a?)">);
    static_assert(accepted<R"(f?[a-z-]?a?)">);
    static_assert(accepted<R"(f?[a-z-A]?a?)">);
    static_assert(accepted<R"(f?[a-zA-]?a?)">);
    static_assert(accepted<R"(f??[a-z-A]??a?)">);
    static_assert(accepted<R"(f?+[a-z-A]*+a?)">);
    static_assert(accepted<R"(f*[a-zA-Z]*a?)">);
    static_assert(accepted<R"(f+[a-zA-Z]+a+)">);
    static_assert(accepted<R"(f?[a-zA-Z]?a?[xyzt\w0-9]+d?)">);
    static_assert(accepted<R"(])">);
    static_assert(accepted<R"([]])">);
    static_assert(accepted<R"([a]{12}[])">);
    static_assert(accepted<R"([][b])">);
    static_assert(accepted<R"([]-z])">);
    static_assert(accepted<R"([- -[])">);
    // Other random patterns
    static_assert(accepted<R"(ab|cd|[a-zA-Z]|ab{3}|ac)">);
    static_assert(accepted<R"(ab|(cd|[a-zA-Z]|ab)|ac)">);
    static_assert(accepted<R"(^(ab|(cd|[a-zA-Z]|ab)|ac)$)">);
    static_assert(accepted<R"(((a|\\a)|(d|\\d))?)">);
    static_assert(accepted<R"(((a|\\a+)|(d?|\\d))*)">);
    static_assert(accepted<R"(((a|\\a)|(0|1|2|3))?)">);
    static_assert(accepted<R"((((a|\\a)|(0|1|2|3))?)*x?y+abcd|efgh((x|z)?t)+)">);
    // Lookarounds
    static_assert(accepted<R"(xyz(?=a)abc)">);
    static_assert(accepted<R"(xyz(?!a)abc)">);
    static_assert(accepted<R"(xyz(?=)abc)">);
    static_assert(accepted<R"(xyz(?!)abc)">);
    static_assert(accepted<R"(xyz(?<=abc)abc)">);
    static_assert(accepted<R"(xyz(?<!abc)abc)">);
    static_assert(accepted<R"(xyz(?<=a)abc)">);
    static_assert(accepted<R"(xyz(?<!a)abc)">);
    static_assert(accepted<R"(xyz(?<=)abc)">);
    static_assert(accepted<R"(xyz(?<!)abc)">);
    // Named groups and named backreferences
    static_assert(accepted<R"(abc(?<test>.[a-d]+)*cd)">);
    static_assert(accepted<R"(abc(?<te2_3>.[a-d]+)+cd)">);
    static_assert(accepted<R"(abc(?<lvl1>.[a-d]+(?<lvl2>x+y*))+cd)">);
    static_assert(accepted<R"(abc(?<____>.[a-d]+(?<_1232>x+y*)+){2,3}cd)">);
    static_assert(accepted<R"(abc(?<____>.[a-d]+(?<_1232>x+y*)+){2,3}cd\k<_1232>+)">);
    static_assert(accepted<R"(abc(?<____>.[a-d]+(?<_1232>\k<____>{2}x+y*)+){2,3}cd)">);
    static_assert(accepted<R"((?<foo>.)\k<foo>\1)">);
    static_assert(accepted<R"((?<foo>.+?)\k<foo>*?\1)">);

    /**
     * Test invalid inputs
     */

    // Basic patterns
    static_assert(accepted<R"(()"> == false);
    static_assert(accepted<R"())"> == false);
    static_assert(accepted<R"(+)"> == false);
    static_assert(accepted<R"(?)"> == false);
    static_assert(accepted<R"(*)"> == false);
    static_assert(accepted<R"(|)"> == false);
    static_assert(accepted<R"(\)"> == false);
    static_assert(accepted<R"(a|)"> == false);
    static_assert(accepted<R"(|a)"> == false);
    static_assert(accepted<R"(a|))"> == false);
    // Anchors
    static_assert(accepted<R"(^?)"> == false);
    static_assert(accepted<R"(^?)"> == false);
    static_assert(accepted<R"($?)"> == false);
    static_assert(accepted<R"(^$+)"> == false);
    static_assert(accepted<R"(^$*)"> == false);
    static_assert(accepted<R"(^$*?)"> == false);
    static_assert(accepted<R"(^*$?)"> == false);
    static_assert(accepted<R"(^?$??)"> == false);
    static_assert(accepted<R"(\b+)"> == false);
    static_assert(accepted<R"(\b?)"> == false);
    static_assert(accepted<R"(\B?+)"> == false);
    static_assert(accepted<R"(\b+?)"> == false);
    static_assert(accepted<R"(\b{**)"> == false);
    static_assert(accepted<R"(\b{*++)"> == false);
    static_assert(accepted<R"(\b{??+)"> == false);
    // Repetitions
    static_assert(accepted<R"({)"> == false);
    static_assert(accepted<R"({x})"> == false);
    static_assert(accepted<R"({1})"> == false);
    static_assert(accepted<R"(c+{x)"> == false);
    static_assert(accepted<R"(c+{3})"> == false);
    static_assert(accepted<R"(c{2x})"> == false);
    static_assert(accepted<R"(c{123)"> == false);
    static_assert(accepted<R"(c{123\})"> == false);
    static_assert(accepted<R"(c{1,2,3})"> == false);
    static_assert(accepted<R"(c{12,3)"> == false);
    static_assert(accepted<R"(c{0,123,})"> == false);
    static_assert(accepted<R"(c{0, 123})"> == false);
    static_assert(accepted<R"(c{0,123 })"> == false);
    static_assert(accepted<R"(c{1\,2})"> == false);
    // Groups
    static_assert(accepted<R"((|))"> == false);
    static_assert(accepted<R"((+))"> == false);
    static_assert(accepted<R"((?))"> == false);
    static_assert(accepted<R"((*))"> == false);
    static_assert(accepted<R"((())"> == false);
    static_assert(accepted<R"(()))"> == false);
    static_assert(accepted<R"((|a))"> == false);
    static_assert(accepted<R"((a{|))"> == false);
    static_assert(accepted<R"((+|-))"> == false);
    static_assert(accepted<R"(x(?))"> == false);
    static_assert(accepted<R"((?:xx)"> == false);
    static_assert(accepted<R"(?:x)?)"> == false);
    static_assert(accepted<R"(x+(?:?)*)"> == false);
    static_assert(accepted<R"((?:|x))"> == false);
    static_assert(accepted<R"((?:a)|)"> == false);
    static_assert(accepted<R"(|(?:ab))"> == false);
    static_assert(accepted<R"((?x))"> == false);
    static_assert(accepted<R"((?:abc)"> == false);
    static_assert(accepted<R"(xy?((?:abc)zt\)?)"> == false);
    static_assert(accepted<R"((Inner|a)+))"> == false);
    static_assert(accepted<R"(\((Inner|a)+))"> == false);
    static_assert(accepted<R"(((((((a))))))"> == false);
    static_assert(accepted<R"(((((((a)))))\))"> == false);
    static_assert(accepted<R"(((a|\\a)|\(d|\\d))?)"> == false);
    static_assert(accepted<R"(((a|\\a+*)|(d?|\\d))*)"> == false);
    static_assert(accepted<R"(((a|\\a)|(0|1|?2|3))?)"> == false);
    static_assert(accepted<R"((((|\\a)|(0|1|2|3))?)*x?y+abcd|efgh((x|z)?t)+)"> == false);
    // Sets and alternations
    static_assert(accepted<R"([\])"> == false);
    static_assert(accepted<R"([\)"> == false);
    static_assert(accepted<R"([][)"> == false);
    static_assert(accepted<R"([a-\d])"> == false);
    static_assert(accepted<R"([a-\w])"> == false);
    static_assert(accepted<R"([a-\x])"> == false);
    static_assert(accepted<R"([a-\D])"> == false);
    static_assert(accepted<R"([a-\W])"> == false);
    static_assert(accepted<R"([a-\X])"> == false);
    static_assert(accepted<R"([a-\N])"> == false);
    static_assert(accepted<R"([a-\R])"> == false);
    static_assert(accepted<R"([)"> == false);
    static_assert(accepted<R"([a][)"> == false);
    static_assert(accepted<R"(?[a-zA-Z]?a?)"> == false);
    static_assert(accepted<R"(ab||[a-zA-Z])"> == false);
    static_assert(accepted<R"(ab|(cd|[a-zA-Z]|)"> == false);
    static_assert(accepted<R"(f?[a-z-A]?+*)"> == false);
    static_assert(accepted<R"(f*[a-zA-Z*a?)"> == false);
    static_assert(accepted<R"(f**[a-zA-Z])"> == false);
    static_assert(accepted<R"(+[a-zA-Z]+a+)"> == false);
    static_assert(accepted<R"(ab|cd|[a-zA-Z]|ab{3|ac)"> == false);
    static_assert(accepted<R"(ab|(cd|[a-zA-Z]|ab\)|ac)"> == false);
    static_assert(accepted<R"(^(ab||(cd|[a-zA-Z]|ab)|ac)$)"> == false);
    // Lookarounds
    static_assert(accepted<R"(xyz(?<abc)abc)"> == false);
    static_assert(accepted<R"(xyz(?<)abc)"> == false);
    static_assert(accepted<R"(xyz(?<a)abc)"> == false);
    static_assert(accepted<R"(xyz(?<<a)abc)"> == false);
    // Named groups and named backreferences
    static_assert(accepted<R"((?<1name>xyz))"> == false);
    static_assert(accepted<R"((?<namexyz))"> == false);
    static_assert(accepted<R"((?<name >xyz))"> == false);
    static_assert(accepted<R"((?< name>xyz))"> == false);
    static_assert(accepted<R"((?<+name>xyz))"> == false);
    static_assert(accepted<R"(\k<+name>)"> == false);
    static_assert(accepted<R"(\k<name)"> == false);
    static_assert(accepted<R"(\kname>)"> == false);
    static_assert(accepted<R"(\k<1name>)"> == false);
    static_assert(accepted<R"(\k<name>**)"> == false);
    static_assert(accepted<R"(\k<)"> == false);
    static_assert(accepted<R"(\k)"> == false);
}