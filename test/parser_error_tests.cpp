#include <mregex.hpp>

namespace meta
{
    namespace
    {
        template<static_string Pattern, std::size_t I>
        inline constexpr bool error_position = std::is_same_v<status_of<Pattern>, parsing::syntax_error<I>>;
    }

    static_assert(error_position<R"(()", 1>);
    static_assert(error_position<R"())", 0>);
    static_assert(error_position<R"([)", 1>);
    static_assert(error_position<R"(+)", 0>);
    static_assert(error_position<R"(|)", 0>);
    static_assert(error_position<R"(?)", 0>);
    static_assert(error_position<R"(a))", 1>);
    static_assert(error_position<R"(a?*)", 2>);
    static_assert(error_position<R"((?a))", 2>);
    static_assert(error_position<R"((?<*))", 3>);
    static_assert(error_position<R"(ab|+cd)", 3>);
    static_assert(error_position<R"(((ab))", 5>);
    static_assert(error_position<R"((?xx))", 2>);
    static_assert(error_position<R"(a{3x})", 3>);
    static_assert(error_position<R"(a{34,x})", 5>);
    static_assert(error_position<R"(a{3,)", 4>);
    static_assert(error_position<R"(a{1,2,3})", 5>);
    static_assert(error_position<R"(a**b)", 2>);
    static_assert(error_position<R"((a(b(c(d))))))", 12>);
    static_assert(error_position<R"((?<1name>x))", 3>);
    static_assert(error_position<R"((?<name+>x))", 7>);
    static_assert(error_position<R"((?<namex))", 8>);
    static_assert(error_position<R"(\k<)", 3>);
    static_assert(error_position<R"(\k<name)", 7>);
    static_assert(error_position<R"(\kname>)", 2>);
}