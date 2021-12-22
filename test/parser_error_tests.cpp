#include "tests.hpp"

#ifdef MREGEX_RUN_PARSER_TESTS
namespace meta
{
    namespace
    {
        template<static_string pattern, std::size_t I>
        inline constexpr auto error_position = std::is_same_v<status_of<pattern>, parsing::syntax_error<I>>;
    }

    static_assert(error_position<"(", 1>);
    static_assert(error_position<")", 0>);
    static_assert(error_position<"+", 0>);
    static_assert(error_position<"|", 0>);
    static_assert(error_position<"?", 0>);
    static_assert(error_position<"()", 1>);
    static_assert(error_position<"a??", 2>);
    static_assert(error_position<"ab|+cd", 3>);
    static_assert(error_position<"((ab)", 5>);
    static_assert(error_position<"(?xx)", 2>);
    static_assert(error_position<"(?=)", 3>);
    static_assert(error_position<"a{3x}", 3>);
    static_assert(error_position<"a{34,x}", 5>);
    static_assert(error_position<"a{3,", 4>);
    static_assert(error_position<"a{1,2,3}", 5>);
    static_assert(error_position<"a**b", 2>);
    static_assert(error_position<"(a(b(c(d)))))", 12>);
}
#endif //MREGEX_RUN_PARSER_TESTS