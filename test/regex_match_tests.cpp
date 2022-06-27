#include <string_view>
#include "tests.hpp"

#ifdef MREGEX_RUN_REGEX_TESTS
namespace meta::tests
{
    // Test matching inputs
    static_assert(regex<R"()">::match(""));
    static_assert(regex<R"(a)">::match("a"));
    static_assert(regex<R"(\a)">::match("b"));
    static_assert(regex<R"(\n)">::match("\n"));
    static_assert(regex<R"(\N)">::match("N"));
    static_assert(regex<R"(\R)">::match("\r"));
    static_assert(regex<R"(\R)">::match("\n"));
    static_assert(regex<R"(\a+)">::match("aBcDefghijklmnopqrstuvqXyZ"));
    static_assert(regex<R"(^a)">::match("a"));
    static_assert(regex<R"(a$)">::match("a"));
    static_assert(regex<R"(^abcd$)">::match("abcd"));
    static_assert(regex<R"(abcdef)">::match("abcdef"));
    static_assert(regex<R"(abc[]?def)">::match("abcdef"));
    static_assert(regex<R"([a])">::match("a"));
    static_assert(regex<R"([\^a]+)">::match("^a"));
    static_assert(regex<R"([^]+)">::match("aA01-^*@#(){}[]\n"));
    static_assert(regex<R"([^\^a]+)">::match("b$"));
    static_assert(regex<R"([a-z]+)">::match("aqwertyz"));
    static_assert(regex<R"([A-Z]+)">::match("AAABBBCCEEFFZZZ"));
    static_assert(regex<R"([0-9]+)">::match("123123123123"));
    static_assert(regex<R"([(|)][^0-9]+[a-z]+)">::match("(xy"));
    static_assert(regex<R"(a?)">::match(""));
    static_assert(regex<R"(a?)">::match("a"));
    static_assert(regex<R"(a*)">::match(""));
    static_assert(regex<R"(a*)">::match("a"));
    static_assert(regex<R"(a*)">::match("aa"));
    static_assert(regex<R"(a*)">::match("aaaaaaaaaaa"));
    static_assert(regex<R"((a*)+)">::match("a"));
    static_assert(regex<R"((a+)*)">::match("a"));
    static_assert(regex<R"((a*)*)">::match("a"));
    static_assert(regex<R"((a+)+)">::match("a"));
    static_assert(regex<R"((a*)+)">::match("aa"));
    static_assert(regex<R"((a+)*)">::match("aa"));
    static_assert(regex<R"((a*)*)">::match("aa"));
    static_assert(regex<R"((a+)+)">::match("aa"));
    static_assert(regex<R"((a*)+)">::match("aaaaaa"));
    static_assert(regex<R"((a+)*)">::match("aaaaaa"));
    static_assert(regex<R"((a*)*)">::match("aaaaaa"));
    static_assert(regex<R"((a*aa)?aa)">::match("aaaaa"));
    static_assert(regex<R"((a+)+)">::match("aaaaaa"));
    static_assert(regex<R"((a*)+)">::match(""));
    static_assert(regex<R"((a+)*)">::match(""));
    static_assert(regex<R"((a*)*)">::match(""));
    static_assert(regex<R"(a*)", flag::U>::match(""));
    static_assert(regex<R"(a*)", flag::U>::match("a"));
    static_assert(regex<R"(a*)", flag::U>::match("aa"));
    static_assert(regex<R"(a*)", flag::U>::match("aaaaaaaaaaa"));
    static_assert(regex<R"((a*)+)", flag::U>::match("a"));
    static_assert(regex<R"((a+)*)", flag::U>::match("a"));
    static_assert(regex<R"((a*)*)", flag::U>::match("a"));
    static_assert(regex<R"((a+)+)", flag::U>::match("a"));
    static_assert(regex<R"((a*)+)", flag::U>::match("aa"));
    static_assert(regex<R"((a+)*)", flag::U>::match("aa"));
    static_assert(regex<R"((a*)*)", flag::ungreedy>::match("aa"));
    static_assert(regex<R"((a+)+)", flag::ungreedy>::match("aa"));
    static_assert(regex<R"((a*)+)", flag::ungreedy>::match("aaaaaa"));
    static_assert(regex<R"((a+)*)", flag::ungreedy>::match("aaaaaa"));
    static_assert(regex<R"((a*)*)", flag::ungreedy>::match("aaaaaa"));
    static_assert(regex<R"((a+)+)", flag::ungreedy>::match("aaaaaa"));
    static_assert(regex<R"((a*)+)", flag::ungreedy>::match(""));
    static_assert(regex<R"((a+)*)", flag::ungreedy>::match(""));
    static_assert(regex<R"((a*)*)", flag::ungreedy>::match(""));
    static_assert(regex<R"(abc)">::match("abc"));
    static_assert(regex<R"(a{1})">::match("a"));
    static_assert(regex<R"(a\{1})">::match("a{1}"));
    static_assert(regex<R"(a{5})">::match("aaaaa"));
    static_assert(regex<R"(a{0,5})">::match("aaaaa"));
    static_assert(regex<R"(a{1,3})">::match("aaa"));
    static_assert(regex<R"(a{3,3})">::match("aaa"));
    static_assert(regex<R"(a{3,6})">::match("aaa"));
    static_assert(regex<R"(a{1,5})">::match("aaa"));
    static_assert(regex<R"((a?){4,5})">::match("aaa"));
    static_assert(regex<R"((a|bc){0,5})">::match("abcbcbc"));
    static_assert(regex<R"((ab+){3})">::match("ababbbbbab"));
    static_assert(regex<R"((_x){7,17})">::match("_x_x_x_x_x_x_x"));
    static_assert(regex<R"((_x){7,17})">::match("_x_x_x_x_x_x_x_x"));
    static_assert(regex<R"((_x){7,17})">::match("_x_x_x_x_x_x_x_x_x_x_x_x_x_x_x_x"));
    static_assert(regex<R"((_x){7,17})">::match("_x_x_x_x_x_x_x_x_x_x_x_x_x_x_x_x_x"));
    static_assert(regex<R"((abc){0,})">::match("abcabc"));
    static_assert(regex<R"(\w{2}-\w{3}-\w{4})">::match("ab-abc-xyzt"));
    static_assert(regex<R"([a-z]{2})">::match("xy"));
    static_assert(regex<R"(a{0})">::match(""));
    static_assert(regex<R"(a{})">::match("a{}"));
    static_assert(regex<R"(a{x})">::match("a{x}"));
    static_assert(regex<R"((a|b|c){3})">::match("abc"));
    static_assert(regex<R"(a?b?c?)">::match("abc"));
    static_assert(regex<R"(a?b?c?)">::match("a"));
    static_assert(regex<R"(a?b?c?)">::match("b"));
    static_assert(regex<R"(a?b?c?)">::match("c"));
    static_assert(regex<R"(a?b?c?)">::match("ac"));
    static_assert(regex<R"(a+b+c+)">::match("abc"));
    static_assert(regex<R"(a+b+c+)">::match("aaabbcccccc"));
    static_assert(regex<R"((x|y|z)+)">::match("xyzxzyxyzxyzyxyxxxzxxzyxyzx"));
    static_assert(regex<R"(a|ab|abc)">::match("a"));
    static_assert(regex<R"((a|ab)xd)">::match("abxd"));
    static_assert(regex<R"((a|ab)+x)">::match("aaaababx"));
    static_assert(regex<R"(abc|ab|a)">::match("a"));
    static_assert(regex<R"(abc|ab|a)">::match("ab"));
    static_assert(regex<R"(abc|ab|a)">::match("abc"));
    static_assert(regex<R"((abc)+)">::match("abcabcabc"));
    static_assert(regex<R"((a+|b+)x\1)">::match("aaaxaaa"));
    static_assert(regex<R"(.*(.).*\1)">::match("abcdefd"));
    static_assert(regex<R"(.*(.).*\1)", flag::U>::match("abcdefd"));
    static_assert(regex<R"(a*aa)">::match("aa"));
    static_assert(regex<R"(a*aab+bb)">::match("aaaaabbb"));
    static_assert(regex<R"((abc)+|tu ?(xyz)+)">::match("abcabcabc"));
    static_assert(regex<R"((abc)+|tu ?(xyz)+)">::match("tuxyz"));
    static_assert(regex<R"((abc)+|tu ?(xyz ?)+)">::match("tuxyzxyz"));
    static_assert(regex<R"((abc)+|tu ?(xyz ?)+)">::match("tuxyz xyz"));
    static_assert(regex<R"((abc)+|tu ?(xyz ?)+)">::match("tu xyz xyz "));
    static_assert(regex<R"(\w+(\.\w+)?@\w+(\.\w+)?)">::match("example@gmail.com"));
    static_assert(regex<R"(\w+(\.\w+)?@\w+(\.\w+)?)">::match("example.test@gmail.com"));
    static_assert(regex<R"(\w+(\.\w+)?@\w+(\.\w+)?)">::match("cpp@regex"));
    static_assert(regex<R"(\w+(\.\w+)?@\w+(\.\w+)?)">::match("abc123_321bca@gmail.com"));
    static_assert(regex<R"(\(.+\))">::match("(aaabb)"));
    static_assert(regex<R"(\(\.+\))">::match("(...)"));
    static_assert(regex<R"(0(x|X)(\x+)(h|H)?)">::match("0x1234F"));
    static_assert(regex<R"(.+)">::match("this regex will match any input"));
    static_assert(regex<R"((x{2,})\1+)">::match("xxxxxxxxx"));
    static_assert(regex<R"((a*(x|axx))e)">::match("aaaxxe"));
    static_assert(regex<R"((a|ab|abc){3,}bc)", flag::unroll>::match("aabaabcabcabaaabc"));
    static_assert(regex<R"((ab?c?d){3})">::match("abcdacdad"));
    static_assert(regex<R"((a|ab|abc){3,10}bc)">::match("aabaabcabcabaaabc"));
    static_assert(regex<R"([0-Z]+)", flag::i>::match("1234abczABCZ"));
    static_assert(regex<R"([A-Z]+)", flag::icase>::match("aBcDeFiOyZ"));
    static_assert(regex<R"(hello|salut|bonjour)">::with<flag::icase>::match("SaLuT"));
    static_assert(regex<R"(0(x)(\x+)(h)?)">::with<flag::icase>::match("0X0h"));
    static_assert(regex<R"(.+)", flag::s>::match("\neven new lines!"));
    static_assert(regex<R"(.+)">::with<flag::dotall>::match("this regex will match any input"));
    static_assert(regex<R"((a|ab|abc)+x)">::match("abx"));
    static_assert(regex<R"((a|ab|abc)+x)">::match("abcx"));
    static_assert(regex<R"((a|ab|abc)+x)">::match("aabcaaabababcx"));
    static_assert(regex<R"((a|ab|abc){3}x)", flag::unroll>::match("aabcabx"));
    static_assert(regex<R"(a|ab|abc)">::match("a"));
    static_assert(regex<R"(a|ab|abc)">::match("ab"));
    static_assert(regex<R"(a|ab|abc)">::match("abc"));
    static_assert(regex<R"(abc|ab|a)">::match("abc"));
    static_assert(regex<R"(abc|ab|a)">::match("ab"));
    static_assert(regex<R"(abc|ab|a)">::match("a"));
    static_assert(regex<R"((a|ab)+x)">::match("aaaababx"));
    static_assert(regex<R"((a|ab|abc)+x)">::match("ax"));
    static_assert(regex<R"(^abcd$.+$)", flag::m>::match("\nabcd\nabc"));
    static_assert(regex<R"(^abcd$.+$)">::with<flag::m>::match("abcd\nabc\n"));
    static_assert(regex<R"(^abcd$.+$)">::with<flag::m>::match("\nabcd\nabc\n"));
    static_assert(regex<R"(^abcd$)", flag::multiline>::match("\nabcd\n"));
    static_assert(regex<R"(^abcd$)">::with<flag::multiline>::match("abcd\n"));
    static_assert(regex<R"(^abcd$)">::with<flag::multiline>::match("\nabcd"));
    static_assert(regex<R"(a(?=b)\w)">::match("ab"));
    static_assert(regex<R"(.(?!b).{2})">::match("xxx"));
    static_assert(regex<R"(a(?=(\w))\w\1)">::match("abb"));
    static_assert(regex<R"(..(?<=x)a)">::match("_xa"));
    static_assert(regex<R"(..(?<=xy)a)">::match("xya"));
    static_assert(regex<R"(.{9}(?<=ab{2,4}c{3,5}d)test)">::match("abbbccccdtest"));
    static_assert(regex<R"(.{7}(?<=ab{2,4}c{3,5}d)test)">::match("abbcccdtest"));
    static_assert(regex<R"(.{12}(?<!ab{2,4}c{3,5}d)test)">::match("abbbbbcccccdtest"));
    static_assert(regex<R"(.{10}(?<!ab{2,4}c{3,5}d)test)">::match("abbbbccccctest"));
    static_assert(regex<R"(.(?<!a(?!t)).{4})", meta::flag::ungreedy>::match("atest"));
    static_assert(regex<R"(.(?<=a(?=t)).{4})", meta::flag::ungreedy>::match("atest"));
    static_assert(regex<R"((?<!\\)\\(?!\w+).?)">::match(R"(\)"));
    static_assert(regex<R"((?<!\\)\\(?!\w+).)">::match(R"(\?)"));
    static_assert(regex<R"((?<!\\)\\(?!\w+).)">::match(R"(\%)"));
    static_assert(regex<R"((.*)(?<=\1))">::match(R"(abc)"));
    static_assert(regex<R"(abc(?<=(abc)))">::match(R"(abc)"));
    static_assert(regex<R"(.{3}(?<=(.{3}))\1)">::match(R"(abcabc)"));
    static_assert(regex<R"((.*)x(?<=\1x))">::match(R"(abcx)"));
    static_assert(regex<R"((.)(?:(x)|y)\1)">::match("aya"));
    static_assert(regex<R"((?:(x)x|xy)\1)">::match("xy"));
    static_assert(regex<R"((?:(x)x|xy)\1)">::match("xxx"));

    // Test non-matching inputs
    static_assert(regex<R"()">::match("t") == false);
    static_assert(regex<R"(a)">::match("b") == false);
    static_assert(regex<R"(\a)">::match("1") == false);
    static_assert(regex<R"(\n)">::match("n") == false);
    static_assert(regex<R"(\N)">::match("\r") == false);
    static_assert(regex<R"(\N)">::match("\n") == false);
    static_assert(regex<R"(^a)">::match(" a") == false);
    static_assert(regex<R"(a$)">::match("a ") == false);
    static_assert(regex<R"(^abcd$)">::match(" abcd") == false);
    static_assert(regex<R"(abc[]def)">::match("abcdef") == false);
    static_assert(regex<R"(abcdef[^x])">::match("abcdef") == false);
    static_assert(regex<R"([\^a]+)">::match("^ca") == false);
    static_assert(regex<R"([])">::match("a") == false);
    static_assert(regex<R"([])">::match("") == false);
    static_assert(regex<R"([^\^a]+)">::match("bb^$") == false);
    static_assert(regex<R"([a-z]+)">::match("abasbdbAasdbasbdabs") == false);
    static_assert(regex<R"([A-Z]+)">::match("AAAAVVVVVVVVCCCCCCxCD") == false);
    static_assert(regex<R"([0-9]+)">::match("123123f123123") == false);
    static_assert(regex<R"([0-3]+)">::match("1231243123123") == false);
    static_assert(regex<R"([(|)][^0-9]+[a-z]+)">::match("(x") == false);
    static_assert(regex<R"(a?)">::match("b") == false);
    static_assert(regex<R"(a?)">::match("ab") == false);
    static_assert(regex<R"(a*)">::match("aaaaaaaaabaaaa") == false);
    static_assert(regex<R"((a*)+)">::match("aaab") == false);
    static_assert(regex<R"((a+)*)">::match("aaab") == false);
    static_assert(regex<R"((a*)*)">::match("aaaab") == false);
    static_assert(regex<R"((a+)+)">::match("aaab") == false);
    static_assert(regex<R"((a+)+)">::match("") == false);
    static_assert(regex<R"(a{1})">::match("a{1}") == false);
    static_assert(regex<R"(a{1})">::match("aa") == false);
    static_assert(regex<R"(a{1})">::match("b") == false);
    static_assert(regex<R"(a{1})">::match("") == false);
    static_assert(regex<R"(a{5})">::match("aaaba") == false);
    static_assert(regex<R"(a{5})">::match("aaaaaa") == false);
    static_assert(regex<R"(a{5})">::match("aaaa") == false);
    static_assert(regex<R"(a{5})">::match("aaaaa"));
    static_assert(regex<R"(a{0,5})">::match("aaaaaa") == false);
    static_assert(regex<R"(a{2,3})">::match("a") == false);
    static_assert(regex<R"(a{3,3})">::match("aa") == false);
    static_assert(regex<R"(a{3,6})">::match("aaaaaaa") == false);
    static_assert(regex<R"(a{1,5})">::match("") == false);
    static_assert(regex<R"((_x){7,17})">::match("_x_x_x_x_x") == false);
    static_assert(regex<R"((_x){7,17})">::match("_x_x_x_x_x_x_") == false);
    static_assert(regex<R"((_x){7,17})">::match("_x_x_x_x_x_x_x_x_x_xx_x_x_x_x_x_x") == false);
    static_assert(regex<R"((_x){7,17})">::match("_x_x_x_x_x_x_x_x_x_x_x_x_x_x_x_x_x_") == false);
    static_assert(regex<R"((_x){7,17})">::match("_x_x_x_x_x_x_x_x_x_x_x_x_x_x_x_x_x_x") == false);
    static_assert(regex<R"((abc){0,5})">::match("acabc") == false);
    static_assert(regex<R"((a|bc){0,5})">::match("abcbcbcbcaa") == false);
    static_assert(regex<R"((ab+){1,3})">::match("ababbbbbabab") == false);
    static_assert(regex<R"((abc){1,})">::match("") == false);
    static_assert(regex<R"((abc){0,})">::match("abcabcabcabbabc") == false);
    static_assert(regex<R"([a-z]{2})">::match("0y") == false);
    static_assert(regex<R"(a{0})">::match("a") == false);
    static_assert(regex<R"((a|b|c){3})">::match("abc"));
    static_assert(regex<R"((a|ab)*x)">::match("aaaabbabx") == false);
    static_assert(regex<R"(a+)">::match("") == false);
    static_assert(regex<R"(abc)">::match("cba") == false);
    static_assert(regex<R"(a?b?c?)">::match("abbc") == false);
    static_assert(regex<R"(a+b+c+)">::match("ab") == false);
    static_assert(regex<R"((x|y|z)+)">::match("xyzxzyxyzxyzyxyqxxxzxxzyxyzx") == false);
    static_assert(regex<R"((abc)+)">::match("abcabcbc") == false);
    static_assert(regex<R"((a+|b+)x\1)">::match("aaaxbbb") == false);
    static_assert(regex<R"(.*(.).*\1)">::match("abcdef") == false);
    static_assert(regex<R"(a*aa)">::match("a") == false);
    static_assert(regex<R"(a*aab+bb)">::match("aaabb") == false);
    static_assert(regex<R"((abc)+|tu ?(xyz)+)">::match("xyz") == false);
    static_assert(regex<R"((abc)+|tu ?(xyz ?)+)">::match("tuxyxyz") == false);
    static_assert(regex<R"((abc)+|tu ?(xyz ?)+)">::match("tu xyz  xyz ") == false);
    static_assert(regex<R"(\w+(\.\w+)?@\w+(\.\w+)?)">::match("exa.mp.le@gmail.com") == false);
    static_assert(regex<R"(\w+(\.\w+)?@\w+(\.\w+)?)">::match("example.test@gm.ail.com") == false);
    static_assert(regex<R"(\w+(\.\w+)?@\w+(\.\w+)?)">::match("cpp..@regex") == false);
    static_assert(regex<R"(\w+(\.\w+)?@\w+(\.\w+)?)">::match("abc123_321bca.gmail.com") == false);
    static_assert(regex<R"(0(x|X)(\x+)(h|H)?)">::match("01234F") == false);
    static_assert(regex<R"(0(x|X)(\x+)(h|H)?)">::match("X0h") == false);
    static_assert(regex<R"(a.+)">::match("this regex will match any input") == false);
    static_assert(regex<R"(.+)">::match("\nexcept new lines") == false);
    static_assert(regex<R"(a?   b? c?)">::match("abc") == false);
    static_assert(regex<R"((x{2,})\1+)">::match("xxxxxxxxxxx") == false);
    static_assert(regex<R"(0x(\x+)h?)", flag::i>::match("0X012323EJH") == false);
    static_assert(regex<R"([^a-z]+)">::with<flag::icase>::match("AABBBDBDBDBBSABBDBDBABBA") == false);
    static_assert(regex<R"(^abcd$)", flag::m>::match("\nabcd\n\n") == false);
    static_assert(regex<R"(^abcd$.+$)">::with<flag::m>::match("\n\nabcd\n") == false);
    static_assert(regex<R"((a|ab|abc)+x)", flag::multiline>::match("aaaabcbabx") == false);
    static_assert(regex<R"(^abcd$)">::with<flag::multiline>::match("\nabcd\ne") == false);
    static_assert(regex<R"(^abcd$)">::with<flag::multiline>::match("a\nbcd\n") == false);
    static_assert(regex<R"(.+)", flag::s>::match("") == false);
    static_assert(regex<R"(.+x)">::with<flag::dotall>::match("xy") == false);
    static_assert(regex<R"((a|ab|abc)+x)">::match("ab") == false);
    static_assert(regex<R"((a|ab|abc)+)">::match("abcx") == false);
    static_assert(regex<R"((a|ab|abc){2}x)">::match("aabcabx") == false);
    static_assert(regex<R"((a|ab|abc){4}x)">::match("aabcabx") == false);
    static_assert(regex<R"(a|ab|abc)">::match("") == false);
    static_assert(regex<R"(a|ab|abc)">::match("a|ab|abc") == false);
    static_assert(regex<R"(a|ab|abc)">::match("aba") == false);
    static_assert(regex<R"((a|ab)+x)">::match("aaaabcabx") == false);
    static_assert(regex<R"((a|ab|abc)+x)">::match("x") == false);
    static_assert(regex<R"(a(?=b))">::match("ab") == false);
    static_assert(regex<R"(.(?!b).{2})">::match("xbx") == false);
    static_assert(regex<R"(a(?!b)|cb)">::match("ab") == false);
    static_assert(regex<R"(..(?<=x)a)">::match("__a") == false);
    static_assert(regex<R"(..(?<=xy)a)">::match("yxa") == false);
    static_assert(regex<R"(..(?<=xy)a)">::match("yya") == false);
    static_assert(regex<R"(.(?<!\\)\\(?!\w+))">::match(R"(\\)") == false);
    static_assert(regex<R"((?<!\\)\\(?!\w+).)">::match(R"(\a)") == false);
    static_assert(regex<R"(.{7}(?<=ab{2,4}c{3,5}d)test)">::match("abccccdtest") == false);
    static_assert(regex<R"(.{7}(?<=ab{2,4}c{3,5}d)test)">::match("abbcccdtes") == false);
    static_assert(regex<R"(.{7}(?<!ab{2,4}c{3,5}d)test)">::match("abbcccdtest") == false);
    static_assert(regex<R"(.{2}(?<!a(?=t))test)">::match("atest") == false);
    static_assert(regex<R"((.*)(?<!\1))">::match(R"(abc)") == false);
    static_assert(regex<R"((.*)x(?<!\1x))">::match(R"(abcx)") == false);
}
#endif //MREGEX_RUN_REGEX_TESTS