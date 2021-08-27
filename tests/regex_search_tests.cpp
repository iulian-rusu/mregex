#include <string_view>
#include "tests.hpp"

#ifdef META_RUN_REGEX_TESTS
namespace meta::tests
{
    using namespace std::string_view_literals;

    using uri_regex = regex<R"(([a-zA-Z][a-zA-Z0-9]*)://([^ /]+)(/[^ ]*)?)">;
    using email_regex = regex<R"(([^ @]+)@([^ @]+))">;
    using non_empty_regex = regex<R"([^])">;

    static_assert(uri_regex::find_first("https://google.com"sv));
    static_assert(uri_regex::find_first("      https://google.com"sv));
    static_assert(uri_regex::find_first("https://google.com       "sv));
    static_assert(uri_regex::find_first("... https://google.com ..."sv));
    static_assert(uri_regex::find_first("... https://google.com\n ... https://youtube.com ..."sv));
    static_assert(email_regex::find_first("sample@gmail.com"sv));
    static_assert(email_regex::find_first("      sample@gmail.com"sv));
    static_assert(email_regex::find_first("sample@gmail.com      "sv));
    static_assert(email_regex::find_first("... sample@gmail.com ..."sv));
    static_assert(email_regex::find_first("... sample@gmail.com\n ... name.user@service.com ..."sv));
    static_assert(non_empty_regex::find_first(" "sv));
    static_assert(non_empty_regex::find_first("a"sv));
    static_assert(non_empty_regex::find_first("0"sv));
    static_assert(non_empty_regex::find_first("abcd0123@#$%"sv));
    static_assert(regex<"^abc">::find_first("abcdefg"sv));
    static_assert(regex<"abc$">::find_first("... abc"sv));
    static_assert(regex<"^">::find_first("abcdefg"sv));
    static_assert(regex<"$">::find_first("... abc"sv));

    static_assert(uri_regex::find_first("https:// google.com"sv) == false);
    static_assert(uri_regex::find_first(""sv) == false);
    static_assert(uri_regex::find_first("     google.com       "sv) == false);
    static_assert(uri_regex::find_first("... "sv) == false);
    static_assert(email_regex::find_first("samplegmail.com"sv) == false);
    static_assert(email_regex::find_first(""sv) == false);
    static_assert(email_regex::find_first("      samp @gmail.com"sv) == false);
    static_assert(email_regex::find_first("..."sv) == false);
    static_assert(non_empty_regex::find_first(""sv) == false);
    static_assert(regex<"^abc">::find_first(" abcdefg"sv) == false);
    static_assert(regex<"abc$">::find_first("... abc "sv) == false);
    static_assert(regex<" ^">::find_first(""sv) == false);
    static_assert(regex<"$ ">::find_first(""sv) == false);
}
#endif //META_RUN_REGEX_TESTS