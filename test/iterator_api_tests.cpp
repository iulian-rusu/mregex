#include <string_view>
#include "tests.hpp"

#ifdef MREGEX_RUN_REGEX_TESTS
namespace meta::tests
{
    using namespace std::string_view_literals;

    namespace
    {
        template<static_string Pattern, typename... Flags>
        constexpr bool match_reverse(std::string_view sv)
        {
            using re = regex<Pattern, Flags ...>;
            return re::match(sv.crbegin(), sv.crend()) == true;
        }
    }

    static_assert(match_reverse<R"()">(""sv));
    static_assert(match_reverse<R"(a)">("a"sv));
    static_assert(match_reverse<R"(a?)">("a"sv));
    static_assert(match_reverse<R"(ab)">("ba"sv));
    static_assert(match_reverse<R"(olleh)">("hello"sv));
    static_assert(match_reverse<R"(a{2}c{1,3}b{1})">("bcccaa"sv));
    static_assert(match_reverse<R"(a{2}c{1,3}b{1})">("bccaa"sv));
    static_assert(match_reverse<R"(a{2}c{1,3}b{1})">("bcaa"sv));
    static_assert(match_reverse<R"((.+)ab\1)">(" nice ba nice "sv));

    static_assert(match_reverse<R"()">("a"sv) == false);
    static_assert(match_reverse<R"(a)">(""sv) == false);
    static_assert(match_reverse<R"(a?)">("aa"sv) == false);
    static_assert(match_reverse<R"(ab)">("ab"sv) == false);
    static_assert(match_reverse<R"(a{2}c{1,3}b{1})">("bccccaa"sv) == false);
    static_assert(match_reverse<R"(a{2}c{1,3}b{1})">("baa"sv) == false);
    static_assert(match_reverse<R"(a{2}c{1,3}b{1})">("bccca"sv) == false);
    static_assert(match_reverse<R"((.+)ab\1)">("nice ba nice "sv) == false);
}
#endif
