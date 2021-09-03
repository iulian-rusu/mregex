#include "tests.hpp"

#ifdef META_RUN_REGEX_TESTS
namespace meta::tests
{
    namespace detail
    {
        template<typename Regex>
        constexpr std::size_t token_count(std::string_view sv)
        {
            std::size_t count = 0;
            for([[maybe_unused]] auto &&r : Regex::range(sv))
                ++count;
            return count;
        }
    }

    static_assert(detail::token_count<regex<"">>("") == 0);
    static_assert(detail::token_count<regex<"">>("a") == 0);
    static_assert(detail::token_count<regex<"a">>("a") == 1);
    static_assert(detail::token_count<regex<"a">>("") == 0);
    static_assert(detail::token_count<regex<"a">>("a a a") == 3);
    static_assert(detail::token_count<regex<"a">>("aaaaa") == 5);
    static_assert(detail::token_count<regex<"a">>("b") == 0);
    static_assert(detail::token_count<regex<"a*">>("") == 0);
    static_assert(detail::token_count<regex<R"(a*(?:$|\s))">>("aaaa aaa aa a aa aaa aaaa") == 7);
    static_assert(detail::token_count<regex<"a*">>("") == 0);
    static_assert(detail::token_count<regex<"a+">>("") == 0);
    static_assert(detail::token_count<regex<"a+">>("bbb ccc ddd") == 0);
    static_assert(detail::token_count<regex<"a+", flag::ungreedy>>("a aa aaa") == 6);
    static_assert(detail::token_count<regex<"a+">>("a aa aaa") == 3);
    static_assert(detail::token_count<regex<"abc">>("a") == 0);
    static_assert(detail::token_count<regex<"abc">>("abcd abcd") == 2);
    static_assert(detail::token_count<regex<"^abc">>("abcd abcd") == 1);
    static_assert(detail::token_count<regex<"abc$">>("abcd abcd") == 0);
    static_assert(detail::token_count<regex<"abc$">>("abcd abc") == 1);
    static_assert(detail::token_count<regex<"^abc$">>("abcd abc") == 0);
    static_assert(detail::token_count<regex<"^abc$">>("abc") == 1);
    static_assert(detail::token_count<regex<"$abc$">>("abc") == 0);
    static_assert(detail::token_count<regex<"$">>("abc def") == 0);
    static_assert(detail::token_count<regex<"^">>("abcd abcd") == 0);
    static_assert(detail::token_count<regex<"a{3}">>("a aa aaa aaaa a a aaaaaa") == 4);
    static_assert(detail::token_count<regex<"a{2,}b{1,2}">>("aab ab aa bb aaaaaabb aaaaaab") == 3);
    static_assert(detail::token_count<regex<"\\s">>("a b c d\te\rf\ng") == 6);
}
#endif //META_RUN_REGEX_TESTS