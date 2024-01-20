#include <mregex.hpp>

namespace meta::tests
{
    using test_regex = regex<"test">;
    using result_type = match_result_view<test_regex, std::string_view::iterator>;

    namespace
    {
        inline constexpr std::string_view expected_match_content = "test";

        constexpr bool is_expected_match(result_type const &res) noexcept
        {
            return res.matched() &&
                   res.length() == expected_match_content.length() &&
                   !res.group<0>().is_empty() &&
                   res == expected_match_content;
        }

        constexpr bool is_expected_non_match(result_type const &res) noexcept
        {
            return !res.matched() &&
                   res.length() == 0 &&
                   res.group<0>().is_empty();
        }
    }

    static_assert(result_type::is_view);
    static_assert(result_type::group_count == 1);

    static_assert(is_expected_match(test_regex::match("test")));
    static_assert(is_expected_non_match(test_regex::match("xxx")));

    static_assert(is_expected_match(test_regex::match_prefix("test___")));
    static_assert(is_expected_non_match(test_regex::match_prefix("_test")));

    static_assert(is_expected_match(test_regex::search("___test___")));
    static_assert(is_expected_non_match(test_regex::search("xxx")));
}