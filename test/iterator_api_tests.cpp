#include <string_view>
#include "tests.hpp"

#ifdef MREGEX_RUN_REGEX_TESTS
namespace meta::tests
{
    using namespace std::string_view_literals;

    namespace detail
    {
        template<typename Regex>
        constexpr bool match_reverse(std::string_view sv)
        {
            return static_cast<bool>(Regex::match(sv.crbegin(), sv.crend()));
        }
    }

    static_assert(detail::match_reverse<regex<R"()">>(""sv));
    static_assert(detail::match_reverse<regex<R"(a)">>("a"sv));
    static_assert(detail::match_reverse<regex<R"(a?)">>("a"sv));
    static_assert(detail::match_reverse<regex<R"(ab)">>("ba"sv));
    static_assert(detail::match_reverse<regex<R"(olleh)">>("hello"sv));
    static_assert(detail::match_reverse<regex<R"(a{2}c{1,3}b{1})">>("bcccaa"sv));
    static_assert(detail::match_reverse<regex<R"(a{2}c{1,3}b{1})">>("bccaa"sv));
    static_assert(detail::match_reverse<regex<R"(a{2}c{1,3}b{1})">>("bcaa"sv));
    static_assert(detail::match_reverse<regex<R"((.+)ab\1)">>(" nice ba nice "sv));

    static_assert(detail::match_reverse<regex<R"()">>("a"sv) == false);
    static_assert(detail::match_reverse<regex<R"(a)">>(""sv) == false);
    static_assert(detail::match_reverse<regex<R"(a?)">>("aa"sv) == false);
    static_assert(detail::match_reverse<regex<R"(ab)">>("ab"sv) == false);
    static_assert(detail::match_reverse<regex<R"(a{2}c{1,3}b{1})">>("bccccaa"sv) == false);
    static_assert(detail::match_reverse<regex<R"(a{2}c{1,3}b{1})">>("baa"sv) == false);
    static_assert(detail::match_reverse<regex<R"(a{2}c{1,3}b{1})">>("bccca"sv) == false);
    static_assert(detail::match_reverse<regex<R"((.+)ab\1)">>("nice ba nice "sv) == false);
}
#endif
