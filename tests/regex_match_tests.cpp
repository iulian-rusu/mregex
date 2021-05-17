#include "tests.h"
#include <string_view>

#ifdef CX_RUN_REGEX_TESTS
namespace cx::tests
{
    using namespace std::string_view_literals;

    // test matching inputs
    static_assert(regex<"">::match(""sv));
    static_assert(regex<"a">::match("a"sv));
    static_assert(regex<"a?">::match(""sv));
    static_assert(regex<"a?">::match("a"sv));
    static_assert(regex<"a*">::match(""sv));
    static_assert(regex<"a*">::match("a"sv));
    static_assert(regex<"a*">::match("aa"sv));
    static_assert(regex<"a*">::match("aaaaaaaaaaa"sv));
    static_assert(regex<"abc">::match("abc"sv));
    static_assert(regex<"a?b?c?">::match("abc"sv));
    static_assert(regex<"a?b?c?">::match("a"sv));
    static_assert(regex<"a?b?c?">::match("b"sv));
    static_assert(regex<"a?b?c?">::match("c"sv));
    static_assert(regex<"a?b?c?">::match("ac"sv));
    static_assert(regex<"a+b+c+">::match("abc"sv));
    static_assert(regex<"a+b+c+">::match("aaabbcccccc"sv));
    static_assert(regex<"(x|y|z)+">::match("xyzxzyxyzxyzyxyxxxzxxzyxyzx"sv));
    static_assert(regex<"(abc)+">::match("abcabcabc"sv));
    static_assert(regex<"a*aa">::match("aa"sv));
    static_assert(regex<"a*aab+bb">::match("aaaaabbb"sv));
    static_assert(regex<"(abc)+|tu ?(xyz)+">::match("abcabcabc"sv));
    static_assert(regex<"(abc)+|tu ?(xyz)+">::match("tuxyz"sv));
    static_assert(regex<"(abc)+|tu ?(xyz ?)+">::match("tuxyzxyz"sv));
    static_assert(regex<"(abc)+|tu ?(xyz ?)+">::match("tuxyz xyz"sv));
    static_assert(regex<"(abc)+|tu ?(xyz ?)+">::match("tu xyz xyz "sv));
    static_assert(regex<R"(\w+(.\w+)?@\w+(.\w+)?)">::match("example@gmail.com"sv));
    static_assert(regex<R"(\w+(.\w+)?@\w+(.\w+)?)">::match("example.test@gmail.com"sv));
    static_assert(regex<R"(\w+(.\w+)?@\w+(.\w+)?)">::match("cpp@regex"sv));
    static_assert(regex<R"(\w+(.\w+)?@\w+(.\w+)?)">::match("abc123_321bca@gmail.com"sv));
    static_assert(cx::regex<R"(0(x|X)(\h+)(h|H)?)">::match("0x1234F"sv));
    static_assert(cx::regex<R"(0(x|X)(\h+)(h|H)?)">::match("0X0h"sv));
    static_assert(cx::regex<R"(hello|salut|bonjour|привет)">::match("salut"sv));

    // test non-matching inputs
    static_assert(regex<"">::match("t"sv) == false);
    static_assert(regex<"a">::match("b"sv) == false);
    static_assert(regex<"a?">::match("b"sv) == false);
    static_assert(regex<"a?">::match("ab"sv) == false);
    static_assert(regex<"a*">::match("aaaaaaaaabaaaa"sv) == false);
    static_assert(regex<"a+">::match(""sv) == false);
    static_assert(regex<"abc">::match("cba"sv) == false);
    static_assert(regex<"a?b?c?">::match("abbc"sv) == false);
    static_assert(regex<"a+b+c+">::match("ab"sv) == false);
    static_assert(regex<"(x|y|z)+">::match("xyzxzyxyzxyzyxyqxxxzxxzyxyzx"sv) == false);
    static_assert(regex<"(abc)+">::match("abcabcbc"sv) == false);
    static_assert(regex<"a*aa">::match("a"sv) == false);
    static_assert(regex<"a*aab+bb">::match("aaabb"sv) == false);
    static_assert(regex<"(abc)+|tu ?(xyz)+">::match("xyz"sv) == false);
    static_assert(regex<"(abc)+|tu ?(xyz ?)+">::match("tuxyxyz"sv) == false);
    static_assert(regex<"(abc)+|tu ?(xyz ?)+">::match("tu xyz  xyz "sv) == false);
    static_assert(regex<R"(\w+(.\w+)?@\w+(.\w+)?)">::match("exa.mp.le@gmail.com"sv) == false);
    static_assert(regex<R"(\w+(.\w+)?@\w+(.\w+)?)">::match("example.test@gm.ail.com"sv) == false);
    static_assert(regex<R"(\w+(.\w+)?@\w+(.\w+)?)">::match("cpp..@regex"sv) == false);
    static_assert(regex<R"(\w+(.\w+)?@\w+(.\w+)?)">::match("abc123_321bca.gmail.com"sv) == false);
    static_assert(cx::regex<R"(0(x|X)(\h+)(h|H)?)">::match("01234F"sv) == false);
    static_assert(cx::regex<R"(0(x|X)(\h+)(h|H)?)">::match("X0h"sv) == false);
    static_assert(cx::regex<R"(0(x|X)(\h+)(h|H)?)">::match("0x12345GFD"sv) == false);
}
#endif // CX_RUN_REGEX_TESTS
