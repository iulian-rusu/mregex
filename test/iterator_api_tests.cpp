#include <mregex.hpp>

namespace meta::tests
{
    namespace
    {
        template<static_string Pattern, typename... Flags>
        constexpr bool match_reverse(std::string_view sv)
        {
            using re = regex<Pattern, Flags ...>;
            return re::match(sv.crbegin(), sv.crend()) == true;
        }
    }

    static_assert(match_reverse<R"()">(""));
    static_assert(match_reverse<R"(a)">("a"));
    static_assert(match_reverse<R"(a?)">("a"));
    static_assert(match_reverse<R"(ab)">("ba"));
    static_assert(match_reverse<R"(olleh)">("hello"));
    static_assert(match_reverse<R"(a{2}c{1,3}b{1})">("bcccaa"));
    static_assert(match_reverse<R"(a{2}c{1,3}b{1})">("bccaa"));
    static_assert(match_reverse<R"(a{2}c{1,3}b{1})">("bcaa"));
    static_assert(match_reverse<R"((.+)ab\1)">(" nice ba nice "));

    static_assert(match_reverse<R"()">("a") == false);
    static_assert(match_reverse<R"(a)">("") == false);
    static_assert(match_reverse<R"(a?)">("aa") == false);
    static_assert(match_reverse<R"(ab)">("ab") == false);
    static_assert(match_reverse<R"(a{2}c{1,3}b{1})">("bccccaa") == false);
    static_assert(match_reverse<R"(a{2}c{1,3}b{1})">("baa") == false);
    static_assert(match_reverse<R"(a{2}c{1,3}b{1})">("bccca") == false);
    static_assert(match_reverse<R"((.+)ab\1)">("nice ba nice ") == false);
}