#include <string_view>
#include "tests.hpp"

#ifdef META_RUN_REGEX_TESTS
namespace meta::tests
{
    using namespace std::string_view_literals;

    // Test matching inputs
    static_assert(regex<R"()">::match(""sv));
    static_assert(regex<R"(a)">::match("a"sv));
    static_assert(regex<R"(\a)">::match("b"sv));
    static_assert(regex<R"(\n)">::match("\n"sv));
    static_assert(regex<R"(\N)">::match("N"sv));
    static_assert(regex<R"(\a+)">::match("aBcDefghijklmnopqrstuvqXyZ"sv));
    static_assert(regex<R"(^a)">::match("a"sv));
    static_assert(regex<R"(a$)">::match("a"sv));
    static_assert(regex<R"(abcdef)">::match("abcdef"sv));
    static_assert(regex<R"(abc[]?def)">::match("abcdef"sv));
    static_assert(regex<R"(^abcd$)">::match("abcd"sv));
    static_assert(regex<R"([a])">::match("a"sv));
    static_assert(regex<R"([\^a]+)">::match("^a"sv));
    static_assert(regex<R"([^]+)">::match("aA01-^*@#(){}[]\n"sv));
    static_assert(regex<R"([^\^a]+)">::match("b$"sv));
    static_assert(regex<R"([a-z]+)">::match("aqwertyz"sv));
    static_assert(regex<R"([A-Z]+)">::match("AAABBBCCEEFFZZZ"sv));
    static_assert(regex<R"([0-9]+)">::match("123123123123"sv));
    static_assert(regex<R"([(|)][^0-9]+[a-z]+)">::match("(xy"sv));
    static_assert(regex<R"(a?)">::match(""sv));
    static_assert(regex<R"(a?)">::match("a"sv));
    static_assert(regex<R"(a*)">::match(""sv));
    static_assert(regex<R"(a*)">::match("a"sv));
    static_assert(regex<R"(a*)">::match("aa"sv));
    static_assert(regex<R"(a*)">::match("aaaaaaaaaaa"sv));
    static_assert(regex<R"((a*)+)">::match("a"sv));
    static_assert(regex<R"((a+)*)">::match("a"sv));
    static_assert(regex<R"((a*)*)">::match("a"sv));
    static_assert(regex<R"((a+)+)">::match("a"sv));
    static_assert(regex<R"((a*)+)">::match("aa"sv));
    static_assert(regex<R"((a+)*)">::match("aa"sv));
    static_assert(regex<R"((a*)*)">::match("aa"sv));
    static_assert(regex<R"((a+)+)">::match("aa"sv));
    static_assert(regex<R"((a*)+)">::match("aaaaaa"sv));
    static_assert(regex<R"((a+)*)">::match("aaaaaa"sv));
    static_assert(regex<R"((a*)*)">::match("aaaaaa"sv));
    static_assert(regex<R"((a+)+)">::match("aaaaaa"sv));
    static_assert(regex<R"((a*)+)">::match(""sv));
    static_assert(regex<R"((a+)*)">::match(""sv));
    static_assert(regex<R"((a*)*)">::match(""sv));
    static_assert(regex<R"(a*)", flag::U>::match(""sv));
    static_assert(regex<R"(a*)", flag::U>::match("a"sv));
    static_assert(regex<R"(a*)", flag::U>::match("aa"sv));
    static_assert(regex<R"(a*)", flag::U>::match("aaaaaaaaaaa"sv));
    static_assert(regex<R"((a*)+)", flag::U>::match("a"sv));
    static_assert(regex<R"((a+)*)", flag::U>::match("a"sv));
    static_assert(regex<R"((a*)*)", flag::U>::match("a"sv));
    static_assert(regex<R"((a+)+)", flag::U>::match("a"sv));
    static_assert(regex<R"((a*)+)", flag::U>::match("aa"sv));
    static_assert(regex<R"((a+)*)", flag::U>::match("aa"sv));
    static_assert(regex<R"((a*)*)", flag::ungreedy>::match("aa"sv));
    static_assert(regex<R"((a+)+)", flag::ungreedy>::match("aa"sv));
    static_assert(regex<R"((a*)+)", flag::ungreedy>::match("aaaaaa"sv));
    static_assert(regex<R"((a+)*)", flag::ungreedy>::match("aaaaaa"sv));
    static_assert(regex<R"((a*)*)", flag::ungreedy>::match("aaaaaa"sv));
    static_assert(regex<R"((a+)+)", flag::ungreedy>::match("aaaaaa"sv));
    static_assert(regex<R"((a*)+)", flag::ungreedy>::match(""sv));
    static_assert(regex<R"((a+)*)", flag::ungreedy>::match(""sv));
    static_assert(regex<R"((a*)*)", flag::ungreedy>::match(""sv));
    static_assert(regex<R"(abc)">::match("abc"sv));
    static_assert(regex<R"(a{1})">::match("a"sv));
    static_assert(regex<R"(a\{1})">::match("a{1}"sv));
    static_assert(regex<R"(a{5})">::match("aaaaa"sv));
    static_assert(regex<R"(a{0,5})">::match("aaaaa"sv));
    static_assert(regex<R"(a{1,3})">::match("aaa"sv));
    static_assert(regex<R"(a{3,3})">::match("aaa"sv));
    static_assert(regex<R"(a{3,6})">::match("aaa"sv));
    static_assert(regex<R"(a{1,5})">::match("aaa"sv));
    static_assert(regex<R"((a?){4,5})">::match("aaa"sv));
    static_assert(regex<R"((a|bc){0,5})">::match("abcbcbc"sv));
    static_assert(regex<R"((ab+){3})">::match("ababbbbbab"sv));
    static_assert(regex<R"((_x){7,17})">::match("_x_x_x_x_x_x_x"sv));
    static_assert(regex<R"((_x){7,17})">::match("_x_x_x_x_x_x_x_x"sv));
    static_assert(regex<R"((_x){7,17})">::match("_x_x_x_x_x_x_x_x_x_x_x_x_x_x_x_x"sv));
    static_assert(regex<R"((_x){7,17})">::match("_x_x_x_x_x_x_x_x_x_x_x_x_x_x_x_x_x"sv));
    static_assert(regex<R"((abc){0,})">::match("abcabc"sv));
    static_assert(regex<R"(\w{2}-\w{3}-\w{4})">::match("ab-abc-xyzt"sv));
    static_assert(regex<R"([a-z]{2})">::match("xy"sv));
    static_assert(regex<R"(a{0})">::match(""sv));
    static_assert(regex<R"(a{})">::match("a{}"sv));
    static_assert(regex<R"(a{x})">::match("a{x}"sv));
    static_assert(regex<R"((a|b|c){3})">::match("abc"sv));
    static_assert(regex<R"(a?b?c?)">::match("abc"sv));
    static_assert(regex<R"(a?b?c?)">::match("a"sv));
    static_assert(regex<R"(a?b?c?)">::match("b"sv));
    static_assert(regex<R"(a?b?c?)">::match("c"sv));
    static_assert(regex<R"(a?b?c?)">::match("ac"sv));
    static_assert(regex<R"(a+b+c+)">::match("abc"sv));
    static_assert(regex<R"(a+b+c+)">::match("aaabbcccccc"sv));
    static_assert(regex<R"((x|y|z)+)">::match("xyzxzyxyzxyzyxyxxxzxxzyxyzx"sv));
    static_assert(regex<R"(a|ab|abc)">::match("a"sv));
    static_assert(regex<R"((a|ab)xd)">::match("abxd"sv));
    static_assert(regex<R"((a|ab)+x)">::match("aaaababx"sv));
    static_assert(regex<R"(abc|ab|a)">::match("a"sv));
    static_assert(regex<R"(abc|ab|a)">::match("ab"sv));
    static_assert(regex<R"(abc|ab|a)">::match("abc"sv));
    static_assert(regex<R"((abc)+)">::match("abcabcabc"sv));
    static_assert(regex<R"((a+|b+)x\1)">::match("aaaxaaa"sv));
    static_assert(regex<R"(.*(.).*\1)">::match("abcdefd"sv));
    static_assert(regex<R"(.*(.).*\1)", flag::U>::match("abcdefd"sv));
    static_assert(regex<R"(a*aa)">::match("aa"sv));
    static_assert(regex<R"(a*aab+bb)">::match("aaaaabbb"sv));
    static_assert(regex<R"((abc)+|tu ?(xyz)+)">::match("abcabcabc"sv));
    static_assert(regex<R"((abc)+|tu ?(xyz)+)">::match("tuxyz"sv));
    static_assert(regex<R"((abc)+|tu ?(xyz ?)+)">::match("tuxyzxyz"sv));
    static_assert(regex<R"((abc)+|tu ?(xyz ?)+)">::match("tuxyz xyz"sv));
    static_assert(regex<R"((abc)+|tu ?(xyz ?)+)">::match("tu xyz xyz "sv));
    static_assert(regex<R"(\w+(\.\w+)?@\w+(\.\w+)?)">::match("example@gmail.com"sv));
    static_assert(regex<R"(\w+(\.\w+)?@\w+(\.\w+)?)">::match("example.test@gmail.com"sv));
    static_assert(regex<R"(\w+(\.\w+)?@\w+(\.\w+)?)">::match("cpp@regex"sv));
    static_assert(regex<R"(\w+(\.\w+)?@\w+(\.\w+)?)">::match("abc123_321bca@gmail.com"sv));
    static_assert(regex<R"(\(.+\))">::match("(aaabb)"sv));
    static_assert(regex<R"(\(\.+\))">::match("(...)"sv));
    static_assert(regex<R"(0(x|X)(\h+)(h|H)?)">::match("0x1234F"sv));
    static_assert(regex<R"(.+)">::match("this regex will match any input"sv));
    static_assert(regex<R"((x{2,})\1+)">::match("xxxxxxxxx"sv));
    static_assert(regex<R"((a*(x|axx))e)">::match("aaaxxe"sv));
    static_assert(regex<R"((a|ab|abc){3,}bc)">::match("aabaabcabcabaaabc"sv));
    static_assert(regex<R"((a|ab|abc){3,10}bc)">::match("aabaabcabcabaaabc"sv));
    static_assert(regex<R"([0-Z]+)", flag::i>::match("1234abczABCZ"sv));
    static_assert(regex<R"([A-Z]+)", flag::ignore_case>::match("aBcDeFiOyZ"sv));
    static_assert(regex<R"(hello|salut|bonjour)">::with<flag::ignore_case>::match("SaLuT"sv));
    static_assert(regex<R"(0(x)(\h+)(h)?)">::with<flag::ignore_case>::match("0X0h"sv));
    static_assert(regex<R"(.+)", flag::s>::match("\neven new lines!"sv));
    static_assert(regex<R"(.+)">::with<flag::dotall>::match("this regex will match any input"sv));
    static_assert(regex<R"((a|ab|abc)+x)">::match("abx"sv));
    static_assert(regex<R"((a|ab|abc)+x)">::match("abcx"sv));
    static_assert(regex<R"((a|ab|abc)+x)">::match("aabcaaabababcx"sv));
    static_assert(regex<R"((a|ab|abc){3}x)">::match("aabcabx"sv));
    static_assert(regex<R"(a|ab|abc)">::match("a"sv));
    static_assert(regex<R"(a|ab|abc)">::match("ab"sv));
    static_assert(regex<R"(a|ab|abc)">::match("abc"sv));
    static_assert(regex<R"(abc|ab|a)">::match("abc"sv));
    static_assert(regex<R"(abc|ab|a)">::match("ab"sv));
    static_assert(regex<R"(abc|ab|a)">::match("a"sv));
    static_assert(regex<R"((a|ab)+x)">::match("aaaababx"sv));
    static_assert(regex<R"((a|ab|abc)+x)">::match("ax"sv));
    static_assert(regex<R"(^abcd$.+$)", flag::m>::match("\nabcd\nabc"sv));
    static_assert(regex<R"(^abcd$.+$)">::with<flag::m>::match("abcd\nabc\n"sv));
    static_assert(regex<R"(^abcd$.+$)">::with<flag::m>::match("\nabcd\nabc\n"sv));
    static_assert(regex<R"(^abcd$)", flag::multiline>::match("\nabcd\n"sv));
    static_assert(regex<R"(^abcd$)">::with<flag::multiline>::match("abcd\n"sv));
    static_assert(regex<R"(^abcd$)">::with<flag::multiline>::match("\nabcd"sv));

    // Test non-matching inputs
    static_assert(regex<R"()">::match("t"sv) == false);
    static_assert(regex<R"(a)">::match("b"sv) == false);
    static_assert(regex<R"(\a)">::match("1"sv) == false);
    static_assert(regex<R"(\n)">::match("n"sv) == false);
    static_assert(regex<R"(\N)">::match("\r"sv) == false);
    static_assert(regex<R"(^a)">::match(" a"sv) == false);
    static_assert(regex<R"(a$)">::match("a "sv) == false);
    static_assert(regex<R"(^abcd$)">::match(" abcd"sv) == false);
    static_assert(regex<R"(abc[]def)">::match("abcdef"sv) == false);
    static_assert(regex<R"(abcdef[^x])">::match("abcdef"sv) == false);
    static_assert(regex<R"([\^a]+)">::match("^ca"sv) == false);
    static_assert(regex<R"([])">::match("a"sv) == false);
    static_assert(regex<R"([])">::match(""sv) == false);
    static_assert(regex<R"([^\^a]+)">::match("bb^$"sv) == false);
    static_assert(regex<R"([a-z]+)">::match("abasbdbAasdbasbdabs"sv) == false);
    static_assert(regex<R"([A-Z]+)">::match("AAAAVVVVVVVVCCCCCCxCD"sv) == false);
    static_assert(regex<R"([0-9]+)">::match("123123f123123"sv) == false);
    static_assert(regex<R"([0-3]+)">::match("1231243123123"sv) == false);
    static_assert(regex<R"([(|)][^0-9]+[a-z]+)">::match("(x"sv) == false);
    static_assert(regex<R"(a?)">::match("b"sv) == false);
    static_assert(regex<R"(a?)">::match("ab"sv) == false);
    static_assert(regex<R"(a*)">::match("aaaaaaaaabaaaa"sv) == false);
    static_assert(regex<R"((a*)+)">::match("aaab"sv) == false);
    static_assert(regex<R"((a+)*)">::match("aaab"sv) == false);
    static_assert(regex<R"((a*)*)">::match("aaaab"sv) == false);
    static_assert(regex<R"((a+)+)">::match("aaab"sv) == false);
    static_assert(regex<R"((a+)+)">::match(""sv) == false);
    static_assert(regex<R"(a{1})">::match("a{1}"sv) == false);
    static_assert(regex<R"(a{1})">::match("aa"sv) == false);
    static_assert(regex<R"(a{1})">::match("b"sv) == false);
    static_assert(regex<R"(a{1})">::match(""sv) == false);
    static_assert(regex<R"(a{5})">::match("aaaba"sv) == false);
    static_assert(regex<R"(a{5})">::match("aaaaaa"sv) == false);
    static_assert(regex<R"(a{5})">::match("aaaa"sv) == false);
    static_assert(regex<R"(a{5})">::match("aaaaa"sv));
    static_assert(regex<R"(a{0,5})">::match("aaaaaa"sv) == false);
    static_assert(regex<R"(a{2,3})">::match("a"sv) == false);
    static_assert(regex<R"(a{3,3})">::match("aa"sv) == false);
    static_assert(regex<R"(a{3,6})">::match("aaaaaaa"sv) == false);
    static_assert(regex<R"(a{1,5})">::match(""sv) == false);
    static_assert(regex<R"((_x){7,17})">::match("_x_x_x_x_x"sv) == false);
    static_assert(regex<R"((_x){7,17})">::match("_x_x_x_x_x_x_"sv) == false);
    static_assert(regex<R"((_x){7,17})">::match("_x_x_x_x_x_x_x_x_x_xx_x_x_x_x_x_x"sv) == false);
    static_assert(regex<R"((_x){7,17})">::match("_x_x_x_x_x_x_x_x_x_x_x_x_x_x_x_x_x_"sv) == false);
    static_assert(regex<R"((_x){7,17})">::match("_x_x_x_x_x_x_x_x_x_x_x_x_x_x_x_x_x_x"sv) == false);
    static_assert(regex<R"((abc){0,5})">::match("acabc"sv) == false);
    static_assert(regex<R"((a|bc){0,5})">::match("abcbcbcbcaa"sv) == false);
    static_assert(regex<R"((ab+){1,3})">::match("ababbbbbabab"sv) == false);
    static_assert(regex<R"((abc){1,})">::match(""sv) == false);
    static_assert(regex<R"((abc){0,})">::match("abcabcabcabbabc"sv) == false);
    static_assert(regex<R"([a-z]{2})">::match("0y"sv) == false);
    static_assert(regex<R"(a{0})">::match("a"sv) == false);
    static_assert(regex<R"((a|b|c){3})">::match("abc"sv));
    static_assert(regex<R"((a|ab)*x)">::match("aaaabbabx"sv) == false);
    static_assert(regex<R"(a+)">::match(""sv) == false);
    static_assert(regex<R"(abc)">::match("cba"sv) == false);
    static_assert(regex<R"(a?b?c?)">::match("abbc"sv) == false);
    static_assert(regex<R"(a+b+c+)">::match("ab"sv) == false);
    static_assert(regex<R"((x|y|z)+)">::match("xyzxzyxyzxyzyxyqxxxzxxzyxyzx"sv) == false);
    static_assert(regex<R"((abc)+)">::match("abcabcbc"sv) == false);
    static_assert(regex<R"((a+|b+)x\1)">::match("aaaxbbb"sv) == false);
    static_assert(regex<R"(.*(.).*\1)">::match("abcdef"sv) == false);
    static_assert(regex<R"(a*aa)">::match("a"sv) == false);
    static_assert(regex<R"(a*aab+bb)">::match("aaabb"sv) == false);
    static_assert(regex<R"((abc)+|tu ?(xyz)+)">::match("xyz"sv) == false);
    static_assert(regex<R"((abc)+|tu ?(xyz ?)+)">::match("tuxyxyz"sv) == false);
    static_assert(regex<R"((abc)+|tu ?(xyz ?)+)">::match("tu xyz  xyz "sv) == false);
    static_assert(regex<R"(\w+(\.\w+)?@\w+(\.\w+)?)">::match("exa.mp.le@gmail.com"sv) == false);
    static_assert(regex<R"(\w+(\.\w+)?@\w+(\.\w+)?)">::match("example.test@gm.ail.com"sv) == false);
    static_assert(regex<R"(\w+(\.\w+)?@\w+(\.\w+)?)">::match("cpp..@regex"sv) == false);
    static_assert(regex<R"(\w+(\.\w+)?@\w+(\.\w+)?)">::match("abc123_321bca.gmail.com"sv) == false);
    static_assert(regex<R"(0(x|X)(\h+)(h|H)?)">::match("01234F"sv) == false);
    static_assert(regex<R"(0(x|X)(\h+)(h|H)?)">::match("X0h"sv) == false);
    static_assert(regex<R"(a.+)">::match("this regex will match any input"sv) == false);
    static_assert(regex<R"(.+)">::match("\nexcept new lines"sv) == false);
    static_assert(regex<R"(a?   b? c?)">::match("abc"sv) == false);
    static_assert(regex<R"((x{2,})\1+)">::match("xxxxxxxxxxx"sv) == false);
    static_assert(regex<R"(0x(\h+)h?)", flag::i>::match("0X012323EJH"sv) == false);
    static_assert(regex<R"([^a-z]+)">::with<flag::ignore_case>::match("AABBBDBDBDBBSABBDBDBABBA"sv) == false);
    static_assert(regex<R"(^abcd$)", flag::m>::match("\nabcd\n\n"sv) == false);
    static_assert(regex<R"(^abcd$.+$)">::with<flag::m>::match("\n\nabcd\n"sv) == false);
    static_assert(regex<R"((a|ab|abc)+x)", flag::multiline>::match("aaaabcbabx"sv) == false);
    static_assert(regex<R"(^abcd$)">::with<flag::multiline>::match("\nabcd\ne"sv) == false);
    static_assert(regex<R"(^abcd$)">::with<flag::multiline>::match("a\nbcd\n"sv) == false);
    static_assert(regex<R"(.+)", flag::s>::match(""sv) == false);
    static_assert(regex<R"(.+x)">::with<flag::dotall>::match("xy"sv) == false);
    static_assert(regex<R"((a|ab|abc)+x)">::match("ab"sv) == false);
    static_assert(regex<R"((a|ab|abc)+)">::match("abcx"sv) == false);
    static_assert(regex<R"((a|ab|abc){2}x)">::match("aabcabx"sv) == false);
    static_assert(regex<R"((a|ab|abc){4}x)">::match("aabcabx"sv) == false);
    static_assert(regex<R"(a|ab|abc)">::match(""sv) == false);
    static_assert(regex<R"(a|ab|abc)">::match("a|ab|abc"sv) == false);
    static_assert(regex<R"(a|ab|abc)">::match("aba"sv) == false);
    static_assert(regex<R"((a|ab)+x)">::match("aaaabcabx"sv) == false);
    static_assert(regex<R"((a|ab|abc)+x)">::match("x"sv) == false);
}
#endif //META_RUN_REGEX_TESTS