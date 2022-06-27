#include <string_view>
#include "tests.hpp"

#ifdef MREGEX_RUN_REGEX_TESTS
namespace meta::tests
{
    using uri_regex = regex<R"(([a-zA-Z][a-zA-Z0-9]*)://([^ /]+)(/[^ ]*)?)">;
    using email_regex = regex<R"(([^ @]+)@([^ @]+))">;
    using non_empty_regex = regex<R"([^])">;

    static_assert(uri_regex::search("https://google.com"));
    static_assert(uri_regex::search("      https://google.com"));
    static_assert(uri_regex::search("https://google.com       "));
    static_assert(uri_regex::search("... https://google.com ..."));
    static_assert(uri_regex::search("... https://google.com\n ... https://youtube.com ..."));
    static_assert(email_regex::search("sample@gmail.com"));
    static_assert(email_regex::search("      sample@gmail.com"));
    static_assert(email_regex::search("sample@gmail.com      "));
    static_assert(email_regex::search("... sample@gmail.com ..."));
    static_assert(email_regex::search("... sample@gmail.com\n ... name.user@service.com ..."));
    static_assert(non_empty_regex::search(" "));
    static_assert(non_empty_regex::search("a"));
    static_assert(non_empty_regex::search("0"));
    static_assert(non_empty_regex::search("abcd0123@#$%"));
    static_assert(regex<"^abc">::search("abcdefg"));
    static_assert(regex<"abc$">::search("... abc"));
    static_assert(regex<"^">::search("abcdefg"));
    static_assert(regex<"$">::search("... abc"));
    static_assert(regex<"^">::search(""));
    static_assert(regex<"$">::search(""));
    static_assert(regex<"^$">::search(""));
    static_assert(regex<"^$">::search(""));

    static_assert(uri_regex::search("https:// google.com") == false);
    static_assert(uri_regex::search("") == false);
    static_assert(uri_regex::search("     google.com       ") == false);
    static_assert(uri_regex::search("... ") == false);
    static_assert(email_regex::search("samplegmail.com") == false);
    static_assert(email_regex::search("") == false);
    static_assert(email_regex::search("      samp @gmail.com") == false);
    static_assert(email_regex::search("...") == false);
    static_assert(non_empty_regex::search("") == false);
    static_assert(regex<"^abc">::search(" abcdefg") == false);
    static_assert(regex<"abc$">::search("... abc ") == false);
    static_assert(regex<" ^">::search("") == false);
    static_assert(regex<"$ ">::search("") == false);
}
#endif //MREGEX_RUN_REGEX_TESTS