#include "tests.h"
#include <string_view>

#ifdef CX_RUN_REGEX_UNION_TESTS
namespace cx::tests
{
    using namespace std::string_view_literals;

    // Test capture count
    static_assert(regex_union<R"()">::capture_count == 0);
    static_assert(regex_union<R"()", R"(a(bc)d)">::capture_count == 1);
    static_assert(regex_union<R"()", R"(a(bc)d)", R"(a(bd(de))(fg))">::capture_count == 3);
    static_assert(regex_union<R"()", R"(a(?:bc)d)">::capture_count == 0);
    static_assert(regex_union<R"()", R"(a(bc)d)", R"(a(?:bd(de))(fg))">::capture_count == 2);
    static_assert(regex_union<R"((((((a))))))", R"(a(bc)d)", R"(a(bd(de))(fg))">::capture_count == 5);
    static_assert(regex_union<R"()", R"(a(bc)d)", R"(a(bd(de))(fg))", R"(((((?:((a)))))))">::capture_count == 5);

    // Test matching inputs
    static_assert(regex_union<R"()">::match(""sv));
    static_assert(regex_union<R"(a)">::match("a"sv));
    static_assert(regex_union<R"()", R"(a)", R"(abc)">::match(""sv));
    static_assert(regex_union<R"()", R"(a)", R"(abc)">::match("a"sv));
    static_assert(regex_union<R"()", R"(a)", R"(abc)">::match("abc"sv));
    static_assert(regex_union<R"(a(bc)+d)", R"(x(y|z)t)", R"(\d+)">::match("abcd"sv));
    static_assert(regex_union<R"(a(bc)+d)", R"(x(y|z)t)", R"(\d+)">::match("abcbcd"sv));
    static_assert(regex_union<R"(a(bc)+d)", R"(x(y|z)t)", R"(\d+)">::match("xyt"sv));
    static_assert(regex_union<R"(a(bc)+d)", R"(x(y|z)t)", R"(\d+)">::match("xzt"sv));
    static_assert(regex_union<R"(a(bc)+d)", R"(x(y|z)t)", R"(\d+)">::match("0"sv));
    static_assert(regex_union<R"(a(bc)+d)", R"(x(y|z)t)", R"(\d+)">::match("01"sv));
    static_assert(regex_union<R"(a(bc)+d)", R"(x(y|z)t)", R"(\d+)">::match("012312324"sv));
    static_assert(regex_union<R"(in)", R"(intern)", R"(internal)">::match("internal"sv));
    static_assert(regex_union<R"(in)", R"(intern)", R"(internal)">::match("internal"sv));
    static_assert(regex_union<R"(in)", R"(intern)", R"(internal)">::match("intern"sv));
    static_assert(regex_union<R"(in)", R"(intern)", R"(internal)">::match("in"sv));
    static_assert(regex_union<R"(aB+b)", R"(B+)", R"(a?Ab?)">::match("aBBBBb"sv));
    static_assert(regex_union<R"(aB+b)", R"(A+)", R"(a?Ab?)">::match("AAAAA"sv));
    static_assert(regex_union<R"(aB+b)", R"(B+)", R"(a?Ab?)">::match("aA"sv));
    static_assert(regex_union<R"(aB+b)", R"(B+)", R"(a?Ab?)">::match("Ab"sv));
    static_assert(regex_union<R"(aB+b)", R"(A+)", R"(a?Ab?)">::with_flags<flag::i>::match("aaaaaaa"sv));

    // Test non-matching inputs
    static_assert(regex_union<R"()">::match("a"sv) == false);
    static_assert(regex_union<R"(a)">::match(""sv) == false);
    static_assert(regex_union<R"()", R"(a)", R"(abc)">::match("acb"sv) == false);
    static_assert(regex_union<R"()", R"(a)", R"(abc)">::match("aaabc"sv) == false);
    static_assert(regex_union<R"(a(bc)+d)", R"(x(y|z)t)", R"(\d+)">::match("abcbd"sv) == false);
    static_assert(regex_union<R"(a(bc)+d)", R"(x(y|z)t)", R"(\d+)">::match(""sv) == false);
    static_assert(regex_union<R"(a(bc)+d)", R"(x(y|z)t)", R"(\d+)">::match("0123abcd12324"sv) == false);
    static_assert(regex_union<R"(in)", R"(intern)", R"(internal)">::match("inter"sv) == false);
    static_assert(regex_union<R"(aB+b)", R"(A+)", R"(a?Ab?)">::match("aaaaaaa"sv) == false);
}
#endif // CX_RUN_REGEX_UNION_TESTS

