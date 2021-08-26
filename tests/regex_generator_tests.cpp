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
            for([[maybe_unused]] auto &&r : Regex::find_all(sv))
                ++count;
            return count;
        }
    }

    static_assert(detail::token_count<regex<"">>("") == 0);
    static_assert(detail::token_count<regex<"">>("a") == 0);
    static_assert(detail::token_count<regex<"a">>("a") == 1);
    static_assert(detail::token_count<regex<"a">>("") == 0);
    static_assert(detail::token_count<regex<"a">>("a a a") == 3);
    static_assert(detail::token_count<regex<"a">>("aaaaaaa") == 7);
    static_assert(detail::token_count<regex<"a">>("b") == 0);
    static_assert(detail::token_count<regex<"a*">>("") == 0);
    static_assert(detail::token_count<regex<"a*">>("aaaa aaa aa a aa aaa aaaa") == 7);
    static_assert(detail::token_count<regex<"a*">>("") == 0);
    static_assert(detail::token_count<regex<"a+">>("") == 0);
    static_assert(detail::token_count<regex<"a+">>("bcde fghi") == 0);
    static_assert(detail::token_count<regex<"a+">>("a aaa aaaaaa") == 3);
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
    static_assert(detail::token_count<regex<"a{3}">>("a aa aaa aaa a a aaa aa aaa") == 4);
    static_assert(detail::token_count<regex<"a{2,}b{1,2}">>("aab ab aa bb aaaaaabb aaaaaab") == 3);
    static_assert(detail::token_count<regex<"\\s">>("a b c d\te\rf\ng") == 6);
}
#endif //META_RUN_REGEX_TESTS