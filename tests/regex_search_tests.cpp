#include <string_view>
#include "tests.hpp"

#ifdef META_RUN_REGEX_TESTS
namespace meta::tests
{
    using namespace std::string_view_literals;

    using uri_regex = regex<R"(([a-zA-Z][a-zA-Z0-9]*)://([^ /]+)(/[^ ]*)?)">;
    using email_regex = regex<R"(([^ @]+)@([^ @]+))">;

    static_assert(uri_regex::find_first("https://google.com"sv));
    static_assert(uri_regex::find_first("        https://google.com"sv));
    static_assert(uri_regex::find_first("https://google.com       "sv));
    static_assert(uri_regex::find_first("... https://google.com ..."sv));
    static_assert(uri_regex::find_first("... https://google.com\n... https://youtube.com ..."sv));

    static_assert(email_regex::find_first("sample@gmail.com"sv));
    static_assert(email_regex::find_first("      sample@gmail.com"sv));
    static_assert(email_regex::find_first("sample@gmail.com      "sv));
    static_assert(email_regex::find_first("... sample@gmail.com ..."sv));
    static_assert(email_regex::find_first("... sample@gmail.com\n... name.user@service.com ..."sv));

    static_assert(uri_regex::find_first("https:// google.com"sv) == false);
    static_assert(uri_regex::find_first(""sv) == false);
    static_assert(uri_regex::find_first("     google.com       "sv) == false);
    static_assert(uri_regex::find_first("... "sv) == false);

    static_assert(email_regex::find_first("samplegmail.com"sv) == false);
    static_assert(email_regex::find_first(""sv) == false);
    static_assert(email_regex::find_first("      samp @gmail.com"sv) == false);
    static_assert(email_regex::find_first("... "sv) == false);
}
#endif //META_RUN_REGEX_TESTS