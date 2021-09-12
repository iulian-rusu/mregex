#include <string_view>
#include "tests.hpp"

#ifdef MREGEX_RUN_REGEX_TESTS
namespace meta::tests
{
    using namespace std::string_view_literals;

    using uri_regex = regex<R"(([a-zA-Z][a-zA-Z0-9]*)://([^ /]+)(/[^ ]*)?)">;
    using email_regex = regex<R"(([^ @]+)@([^ @]+))">;
    using non_empty_regex = regex<R"([^])">;

    static_assert(uri_regex::search("https://google.com"sv));
    static_assert(uri_regex::search("      https://google.com"sv));
    static_assert(uri_regex::search("https://google.com       "sv));
    static_assert(uri_regex::search("... https://google.com ..."sv));
    static_assert(uri_regex::search("... https://google.com\n ... https://youtube.com ..."sv));
    static_assert(email_regex::search("sample@gmail.com"sv));
    static_assert(email_regex::search("      sample@gmail.com"sv));
    static_assert(email_regex::search("sample@gmail.com      "sv));
    static_assert(email_regex::search("... sample@gmail.com ..."sv));
    static_assert(email_regex::search("... sample@gmail.com\n ... name.user@service.com ..."sv));
    static_assert(non_empty_regex::search(" "sv));
    static_assert(non_empty_regex::search("a"sv));
    static_assert(non_empty_regex::search("0"sv));
    static_assert(non_empty_regex::search("abcd0123@#$%"sv));
    static_assert(regex<"^abc">::search("abcdefg"sv));
    static_assert(regex<"abc$">::search("... abc"sv));
    static_assert(regex<"^">::search("abcdefg"sv));
    static_assert(regex<"$">::search("... abc"sv));

    static_assert(uri_regex::search("https:// google.com"sv) == false);
    static_assert(uri_regex::search(""sv) == false);
    static_assert(uri_regex::search("     google.com       "sv) == false);
    static_assert(uri_regex::search("... "sv) == false);
    static_assert(email_regex::search("samplegmail.com"sv) == false);
    static_assert(email_regex::search(""sv) == false);
    static_assert(email_regex::search("      samp @gmail.com"sv) == false);
    static_assert(email_regex::search("..."sv) == false);
    static_assert(non_empty_regex::search(""sv) == false);
    static_assert(regex<"^abc">::search(" abcdefg"sv) == false);
    static_assert(regex<"abc$">::search("... abc "sv) == false);
    static_assert(regex<" ^">::search(""sv) == false);
    static_assert(regex<"$ ">::search(""sv) == false);
}
#endif //MREGEX_RUN_REGEX_TESTS