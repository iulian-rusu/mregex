#include <mregex.hpp>

namespace meta
{
    namespace
    {
        template<static_string Pattern, std::size_t I>
        inline constexpr bool expect_error_at = std::is_same_v<parser_verdict<Pattern>, parsing::syntax_error<I>>;
    }

    static_assert(expect_error_at<R"(()", 1>);
    static_assert(expect_error_at<R"())", 0>);
    static_assert(expect_error_at<R"([)", 1>);
    static_assert(expect_error_at<R"(+)", 0>);
    static_assert(expect_error_at<R"(|)", 0>);
    static_assert(expect_error_at<R"(?)", 0>);
    static_assert(expect_error_at<R"(a))", 1>);
    static_assert(expect_error_at<R"(a?*)", 2>);
    static_assert(expect_error_at<R"((?a))", 2>);
    static_assert(expect_error_at<R"((?<*))", 3>);
    static_assert(expect_error_at<R"(ab|+cd)", 3>);
    static_assert(expect_error_at<R"(((ab))", 5>);
    static_assert(expect_error_at<R"((?xx))", 2>);
    static_assert(expect_error_at<R"(a{3x})", 3>);
    static_assert(expect_error_at<R"(a{34,x})", 5>);
    static_assert(expect_error_at<R"(a{3,)", 4>);
    static_assert(expect_error_at<R"(a{1,2,3})", 5>);
    static_assert(expect_error_at<R"(a**b)", 2>);
    static_assert(expect_error_at<R"((a(b(c(d))))))", 12>);
    static_assert(expect_error_at<R"((?<1name>x))", 3>);
    static_assert(expect_error_at<R"((?<name+>x))", 7>);
    static_assert(expect_error_at<R"((?<namex))", 8>);
    static_assert(expect_error_at<R"(\k<)", 3>);
    static_assert(expect_error_at<R"(\k<name)", 7>);
    static_assert(expect_error_at<R"(\kname>)", 2>);
}