#include "tests.hpp"

#ifdef MREGEX_RUN_REGEX_TESTS
namespace meta::tests
{
    namespace
    {
        template<typename Regex>
        constexpr std::size_t token_count(std::string_view sv)
        {
            std::size_t count = 0;
            for ([[maybe_unused]] auto &&r: Regex::range(sv))
                ++count;
            return count;
        }
    }

    static_assert(token_count<regex<R"()">>("") == 0);
    static_assert(token_count<regex<R"()">>("a") == 0);
    static_assert(token_count<regex<R"(a)">>("a") == 1);
    static_assert(token_count<regex<R"(a)">>("") == 0);
    static_assert(token_count<regex<R"(a)">>("a a a") == 3);
    static_assert(token_count<regex<R"(a)">>("aaaaa") == 5);
    static_assert(token_count<regex<R"(a)">>("b") == 0);
    static_assert(token_count<regex<R"(a*)">>("") == 0);
    static_assert(token_count<regex<R"((a*(?:$|\s)))">>("aaaa aaa aa a aa aaa aaaa") == 7);
    static_assert(token_count<regex<R"(a*)">>("") == 0);
    static_assert(token_count<regex<R"(a+)">>("") == 0);
    static_assert(token_count<regex<R"(a+)">>("bbb ccc ddd") == 0);
    static_assert(token_count<regex<R"(a+)", flag::ungreedy>>("a aa aaa") == 6);
    static_assert(token_count<regex<R"(a+)">>("a aa aaa") == 3);
    static_assert(token_count<regex<R"(abc)">>("a") == 0);
    static_assert(token_count<regex<R"(abc)">>("abcd abcd") == 2);
    static_assert(token_count<regex<R"(^abc)">>("abcd abcd") == 1);
    static_assert(token_count<regex<R"(abc$)">>("abcd abcd") == 0);
    static_assert(token_count<regex<R"(abc$)">>("abcd abc") == 1);
    static_assert(token_count<regex<R"(^abc$)">>("abcd abc") == 0);
    static_assert(token_count<regex<R"(^abc$)">>("abc") == 1);
    static_assert(token_count<regex<R"($abc$)">>("abc") == 0);
    static_assert(token_count<regex<R"($)">>("abc def") == 0);
    static_assert(token_count<regex<R"(^)">>("abcd abcd") == 0);
    static_assert(token_count<regex<R"(a{3})">>("a aa aaa aaaa a a aaaaaa") == 4);
    static_assert(token_count<regex<R"(a{2,}b{1,2})">>("aab ab aa bb aaaaaabb aaaaaab") == 3);
    static_assert(token_count<regex<R"(\s)">>("a b c d\te\rf\ng") == 6);
    static_assert(token_count<regex<R"(.)">>("123456") == 6);
    static_assert(token_count<regex<R"(.)">>("") == 0);
    static_assert(token_count<regex<R"(.)">>("a") == 1);
    static_assert(token_count<regex<R"(.+)">>("123456") == 1);
    static_assert(token_count<regex<R"(.+)", flag::ungreedy>>("123456") == 6);
}
#endif //MREGEX_RUN_REGEX_TESTS