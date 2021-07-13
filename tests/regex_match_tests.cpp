#include "tests.h"
#include <string_view>

#ifdef CX_RUN_REGEX_TESTS
namespace cx::tests
{
    using namespace std::string_view_literals;

    // Test matching inputs
    static_assert(regex<R"()">::match(""sv));
    static_assert(regex<R"(a)">::match("a"sv));
    static_assert(regex<R"(^a)">::match("a"sv));
    static_assert(regex<R"(a$)">::match("a"sv));
    static_assert(regex<R"(abcdef)">::match("abcdef"sv));
    static_assert(regex<R"(abc[]?def)">::match("abcdef"sv));
    static_assert(regex<R"(^abcd$)">::match("abcd"sv));
    static_assert(regex<R"([\^a]+)">::match("^a"sv));
    static_assert(regex<R"([^\^a]+)">::match("b$"sv));
    static_assert(regex<R"([a-z]+)">::match("abasbdbasdbasbdabs"sv));
    static_assert(regex<R"([A-Z]+)">::match("AAAAVVVVVVVVCCCCCCCD"sv));
    static_assert(regex<R"([0-9]+)">::match("123123123123"sv));
    static_assert(regex<R"([(|)][^0-9]+[a-z]+)">::match("(xy"sv));
    static_assert(regex<R"(a?)">::match(""sv));
    static_assert(regex<R"(a?)">::match("a"sv));
    static_assert(regex<R"(a*)">::match(""sv));
    static_assert(regex<R"(a*)">::match("a"sv));
    static_assert(regex<R"(a*)">::match("aa"sv));
    static_assert(regex<R"(a*)">::match("aaaaaaaaaaa"sv));
    static_assert(regex<R"(abc)">::match("abc"sv));
    static_assert(regex<R"(a?b?c?)">::match("abc"sv));
    static_assert(regex<R"(a?b?c?)">::match("a"sv));
    static_assert(regex<R"(a?b?c?)">::match("b"sv));
    static_assert(regex<R"(a?b?c?)">::match("c"sv));
    static_assert(regex<R"(a?b?c?)">::match("ac"sv));
    static_assert(regex<R"(a+b+c+)">::match("abc"sv));
    static_assert(regex<R"(a+b+c+)">::match("aaabbcccccc"sv));
    static_assert(regex<R"((x|y|z)+)">::match("xyzxzyxyzxyzyxyxxxzxxzyxyzx"sv));
    static_assert(regex<R"((abc)+)">::match("abcabcabc"sv));
    static_assert(regex<R"((a+|b+)x\1)">::match("aaaxaaa"sv));
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
    static_assert(regex<R"((?>ab+)c)">::match("abc"sv));
    static_assert(regex<R"((?>[^ @]+)@([^ @]+))">::match("example@gmail.com"sv));
    static_assert(regex<R"(0(x|X)(\h+)(h|H)?)">::match("0x1234F"sv));
    static_assert(regex<R"(0(x|X)(\h+)(h|H)?)">::match("0X0h"sv));
    static_assert(regex<R"(.+)">::match("this regex will match any input"sv));
    static_assert(regex<R"(hello|salut|bonjour)">::match("salut"sv));

    // Test non-matching inputs
    static_assert(regex<R"()">::match("t"sv) == false);
    static_assert(regex<R"(a)">::match("b"sv) == false);
    static_assert(regex<R"(^a)">::match(" a"sv) == false);
    static_assert(regex<R"(a$)">::match("a "sv) == false);
    static_assert(regex<R"(^abcd$)">::match(" abcd"sv) == false);
    static_assert(regex<R"(abc[]def)">::match("abcdef"sv) == false);
    static_assert(regex<R"([\^a]+)">::match("^ca"sv) == false);
    static_assert(regex<R"([^\^a]+)">::match("bb^$"sv) == false);
    static_assert(regex<R"([a-z]+)">::match("abasbdbAasdbasbdabs"sv) == false);
    static_assert(regex<R"([A-Z]+)">::match("AAAAVVVVVVVVCCCCCCxCD"sv) == false);
    static_assert(regex<R"([0-9]+)">::match("123123f123123"sv) == false);
    static_assert(regex<R"([0-3]+)">::match("1231243123123"sv) == false);
    static_assert(regex<R"([(|)][^0-9]+[a-z]+)">::match("(x"sv) == false);
    static_assert(regex<R"(a?)">::match("b"sv) == false);
    static_assert(regex<R"(a?)">::match("ab"sv) == false);
    static_assert(regex<R"(a*)">::match("aaaaaaaaabaaaa"sv) == false);
    static_assert(regex<R"((a|ab)*x)">::match("aaaabbabx"sv) == false);
    static_assert(regex<R"(a+)">::match(""sv) == false);
    static_assert(regex<R"(abc)">::match("cba"sv) == false);
    static_assert(regex<R"(a?b?c?)">::match("abbc"sv) == false);
    static_assert(regex<R"(a+b+c+)">::match("ab"sv) == false);
    static_assert(regex<R"((x|y|z)+)">::match("xyzxzyxyzxyzyxyqxxxzxxzyxyzx"sv) == false);
    static_assert(regex<R"((abc)+)">::match("abcabcbc"sv) == false);
    static_assert(regex<R"((a+|b+)x\1)">::match("aaaxbbb"sv) == false);
    static_assert(regex<R"(a*aa)">::match("a"sv) == false);
    static_assert(regex<R"(a*aab+bb)">::match("aaabb"sv) == false);
    static_assert(regex<R"((abc)+|tu ?(xyz)+)">::match("xyz"sv) == false);
    static_assert(regex<R"((abc)+|tu ?(xyz ?)+)">::match("tuxyxyz"sv) == false);
    static_assert(regex<R"((abc)+|tu ?(xyz ?)+)">::match("tu xyz  xyz "sv) == false);
    static_assert(regex<R"((?>ab+)b)">::match("abb"sv) == false);
    static_assert(regex<R"((?>[^ @]+)@([^ @]+))">::match("example@@gmail.com"sv) == false);
    static_assert(regex<R"(\w+(\.\w+)?@\w+(\.\w+)?)">::match("exa.mp.le@gmail.com"sv) == false);
    static_assert(regex<R"(\w+(\.\w+)?@\w+(\.\w+)?)">::match("example.test@gm.ail.com"sv) == false);
    static_assert(regex<R"(\w+(\.\w+)?@\w+(\.\w+)?)">::match("cpp..@regex"sv) == false);
    static_assert(regex<R"(\w+(\.\w+)?@\w+(\.\w+)?)">::match("abc123_321bca.gmail.com"sv) == false);
    static_assert(regex<R"(0(x|X)(\h+)(h|H)?)">::match("01234F"sv) == false);
    static_assert(regex<R"(0(x|X)(\h+)(h|H)?)">::match("X0h"sv) == false);
    static_assert(regex<R"(0(x|X)(\h+)(h|H)?)">::match("0x12345GFD"sv) == false);
    static_assert(regex<R"(a.+)">::match("this regex will match any input"sv) == false);
}
#endif // CX_RUN_REGEX_TESTS
