#include <mregex.hpp>

namespace meta::tests
{
    namespace
    {
        template<static_string Pattern, regex_flag... Flags>
        constexpr auto match_count(std::string_view sv)
        {
            using pattern = regex<Pattern, Flags ...>;
            return std::ranges::distance(pattern::find_all(sv));
        }

        template<static_string Pattern, regex_flag... Flags>
        constexpr auto token_count(std::string_view sv)
        {
            using pattern = regex<Pattern, Flags ...>;
            return std::ranges::distance(pattern::tokenize(sv));
        }
    }

    static_assert(match_count<R"()">("") == 1);
    static_assert(match_count<R"()">("a") == 1);
    static_assert(match_count<R"(a)">("a") == 1);
    static_assert(match_count<R"(a)">("") == 0);
    static_assert(match_count<R"(a)">("a a a") == 3);
    static_assert(match_count<R"(a)">("aaaaa") == 5);
    static_assert(match_count<R"(a)">("b") == 0);
    static_assert(match_count<R"(a*)">("") == 1);
    static_assert(match_count<R"(a*)">("aa") == 2);
    static_assert(match_count<R"(a+)">("") == 0);
    static_assert(match_count<R"(a+)">("aa") == 1);
    static_assert(match_count<R"(a+)">("a aa aaa") == 3);
    static_assert(match_count<R"(a+)">("bbb ccc ddd") == 0);
    static_assert(match_count<R"(a+)", regex_flag::ungreedy>("a aa aaa") == 6);
    static_assert(match_count<R"(a+?)", regex_flag::ungreedy>("a aa aaa") == 3);
    static_assert(match_count<R"(a+(?:$|\s))">("aaaa aaa aa a aa aaa aaaa") == 7);
    static_assert(match_count<R"(abc)">("a") == 0);
    static_assert(match_count<R"(abc)">("abcd abcd") == 2);
    static_assert(match_count<R"(^abc)">("abcd abcd") == 1);
    static_assert(match_count<R"(abc$)">("abcd abcd") == 0);
    static_assert(match_count<R"(abc$)">("abcd abc") == 1);
    static_assert(match_count<R"(^abc$)">("abcd abc") == 0);
    static_assert(match_count<R"(^abc$)">("abc") == 1);
    static_assert(match_count<R"($abc$)">("abc") == 0);
    static_assert(match_count<R"($)">("abc def") == 1);
    static_assert(match_count<R"(^)">("abcd abcd") == 1);
    static_assert(match_count<R"(a{3})">("a aa aaa aaaa a a aaaaaa") == 4);
    static_assert(match_count<R"(a{2,}b{1,2})">("aab ab aa bb aaaaaabb aaaaaab") == 3);
    static_assert(match_count<R"(\s)">("a b c d\te\rf\ng") == 6);
    static_assert(match_count<R"(.)">("123456") == 6);
    static_assert(match_count<R"(.)">("") == 0);
    static_assert(match_count<R"(.)">("a") == 1);
    static_assert(match_count<R"(.+)">("123456") == 1);
    static_assert(match_count<R"(.+)", regex_flag::ungreedy>("123456") == 6);
    static_assert(match_count<R"(.+?)", regex_flag::ungreedy>("123456") == 1);

    static_assert(token_count<R"()">("") == 1);
    static_assert(token_count<R"()">("a") == 1);
    static_assert(token_count<R"(a)">("a") == 1);
    static_assert(token_count<R"(a)">("") == 0);
    static_assert(token_count<R"(a)">("b") == 0);
    static_assert(token_count<R"(a)">("ababab") == 1);
    static_assert(token_count<R"(a*)">("") == 1);
    static_assert(token_count<R"(a*)">("aa") == 2);
    static_assert(token_count<R"(a+)">("") == 0);
    static_assert(token_count<R"(a+)">("aa") == 1);
    static_assert(token_count<R"(a+)">("a aa aaa") == 1);
    static_assert(token_count<R"([ab])">("ababab") == 6);
    static_assert(token_count<R"(^[ab])">("ababab") == 1);
    static_assert(token_count<R"([ab]$)">("ababab") == 0);
    static_assert(token_count<R"([a-z]|[0-9]{2}|\s)">("a11 b22 c33") == 8);
    static_assert(token_count<R"([a-z]|[0-9]{2}|\s)">("a11 b2_ c33") == 4);
}