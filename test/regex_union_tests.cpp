#include <mregex.hpp>

namespace meta::tests
{
    // Test capture count
    static_assert(regex_union<R"()">::capture_count == 0);
    static_assert(regex_union<R"()", R"(a(bc)d)">::capture_count == 1);
    static_assert(regex_union<R"()", R"(a(bc)d)", R"(a(bd(de))(fg))">::capture_count == 3);
    static_assert(regex_union<R"()", R"(a(?:bc)d)">::capture_count == 0);
    static_assert(regex_union<R"()", R"(a(bc)d)", R"(a(?:bd(de))(fg))">::capture_count == 2);
    static_assert(regex_union<R"((((((a))))))", R"(a(bc)d)", R"(a(bd(de))(fg))">::capture_count == 5);
    static_assert(regex_union<R"()", R"(a(bc)d)", R"(a(bd(de))(fg))", R"(((((?:((a)))))))">::capture_count == 5);

    // Test meta::make_union metafunction
    static_assert(std::is_base_of_v<make_union<regex<R"()">>, regex_union<R"()">>);
    static_assert(std::is_base_of_v<make_union<regex<R"(ab?c+d{3})">>, regex_union<R"(ab?c+d{3})">>);
    static_assert(std::is_base_of_v<make_union<regex<R"(ab+)">, regex<R"(x|z*)">>, regex_union<R"(ab+)", R"(x|z*)">>);

    // Test matching inputs
    static_assert(regex_union<R"()">::match(""));
    static_assert(regex_union<R"(a)">::match("a"));
    static_assert(regex_union<R"()", R"(a)", R"(abc)">::match(""));
    static_assert(regex_union<R"()", R"(a)", R"(abc)">::match("a"));
    static_assert(regex_union<R"()", R"(a)", R"(abc)">::match("abc"));
    static_assert(regex_union<R"(a(bc)+d)", R"(x(y|z)t)", R"(\d+)">::match("abcd"));
    static_assert(regex_union<R"(a(bc)+d)", R"(x(y|z)t)", R"(\d+)">::match("abcbcd"));
    static_assert(regex_union<R"(a(bc)+d)", R"(x(y|z)t)", R"(\d+)">::match("xyt"));
    static_assert(regex_union<R"(a(bc)+d)", R"(x(y|z)t)", R"(\d+)">::match("xzt"));
    static_assert(regex_union<R"(a(bc)+d)", R"(x(y|z)t)", R"(\d+)">::match("0"));
    static_assert(regex_union<R"(a(bc)+d)", R"(x(y|z)t)", R"(\d+)">::match("01"));
    static_assert(regex_union<R"(a(bc)+d)", R"(x(y|z)t)", R"(\d+)">::match("012312324"));
    static_assert(regex_union<R"(in)", R"(intern)", R"(internal)">::match("internal"));
    static_assert(regex_union<R"(in)", R"(intern)", R"(internal)">::match("intern"));
    static_assert(regex_union<R"(in)", R"(intern)", R"(internal)">::match("in"));
    static_assert(regex_union<R"(aB+b)", R"(B+)", R"(a?Ab?)">::match("aBBBBb"));
    static_assert(regex_union<R"(aB+b)", R"(A+)", R"(a?Ab?)">::match("AAAAA"));
    static_assert(regex_union<R"(aB+b)", R"(B+)", R"(a?Ab?)">::match("aA"));
    static_assert(regex_union<R"(aB+b)", R"(B+)", R"(a?Ab?)">::match("Ab"));
    static_assert(regex_union<R"(aB+b)", R"(A+)", R"(a?Ab?)">::with<flag::icase>::match("aaaaaaa"));

    // Test non-matching inputs
    static_assert(regex_union<R"()">::match("a") == false);
    static_assert(regex_union<R"(a)">::match("") == false);
    static_assert(regex_union<R"()", R"(a)", R"(abc)">::match("acb") == false);
    static_assert(regex_union<R"()", R"(a)", R"(abc)">::match("aaabc") == false);
    static_assert(regex_union<R"(a(bc)+d)", R"(x(y|z)t)", R"(\d+)">::match("abcbd") == false);
    static_assert(regex_union<R"(a(bc)+d)", R"(x(y|z)t)", R"(\d+)">::match("") == false);
    static_assert(regex_union<R"(a(bc)+d)", R"(x(y|z)t)", R"(\d+)">::match("0123abcd12324") == false);
    static_assert(regex_union<R"(in)", R"(intern)", R"(internal)">::match("inter") == false);
    static_assert(regex_union<R"(aB+b)", R"(A+)", R"(a?Ab?)">::match("aaaaaaa") == false);
}