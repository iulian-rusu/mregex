#include <mregex.hpp>

namespace meta::tests
{
    namespace
    {
        template<static_string Pattern, regex_flag... Flags>
        constexpr bool match_reverse(std::string_view sv)
        {
            using pattern = regex<Pattern, Flags ...>;
            return pattern::match(sv.crbegin(), sv.crend()) == true;
        }
    }

    static_assert(match_reverse<R"()">(""));
    static_assert(match_reverse<R"(a)">("a"));
    static_assert(match_reverse<R"(a?)">(""));
    static_assert(match_reverse<R"(a?)">("a"));
    static_assert(match_reverse<R"(ab)">("ba"));
    static_assert(match_reverse<R"(abc)">("cba"));
    static_assert(match_reverse<R"(olleh)">("hello"));
    static_assert(match_reverse<R"(\w+(?<=olleh))">("hello"));
    static_assert(match_reverse<R"((?=olleh)\w+)">("hello"));
    static_assert(match_reverse<R"(a{2}c{1,3}b{1})">("bcccaa"));
    static_assert(match_reverse<R"(a{2}c{1,3}b{1})">("bccaa"));
    static_assert(match_reverse<R"(a{2}c{1,3}b{1})">("bcaa"));
    static_assert(match_reverse<R"((.+)ab\1)">("[xyz ]ba[xyz ]"));

    static_assert(match_reverse<R"()">("a") == false);
    static_assert(match_reverse<R"(a)">("") == false);
    static_assert(match_reverse<R"(a?)">("aa") == false);
    static_assert(match_reverse<R"(ab)">("ab") == false);
    static_assert(match_reverse<R"(abc)">("abc") == false);
    static_assert(match_reverse<R"(a{2}c{1,3}b{1})">("bccccaa") == false);
    static_assert(match_reverse<R"(a{2}c{1,3}b{1})">("baa") == false);
    static_assert(match_reverse<R"(a{2}c{1,3}b{1})">("bccca") == false);
    static_assert(match_reverse<R"((.+)ab\1)">("[ xyz]ba[xyz ]") == false);
}