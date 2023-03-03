#include <mregex.hpp>

namespace meta
{
    namespace
    {
        template<static_string Pattern>
        struct has_error
        {
            template< std::size_t I>
            static constexpr bool at_position = std::is_same_v<parser_verdict<Pattern>, parsing::syntax_error<I>>;
        };
    }

    static_assert(has_error<R"(()">::at_position<1>);
    static_assert(has_error<R"())">::at_position<0>);
    static_assert(has_error<R"([)">::at_position<1>);
    static_assert(has_error<R"(+)">::at_position<0>);
    static_assert(has_error<R"(|)">::at_position<0>);
    static_assert(has_error<R"(?)">::at_position<0>);
    static_assert(has_error<R"(a))">::at_position<1>);
    static_assert(has_error<R"(a?*)">::at_position<2>);
    static_assert(has_error<R"((?a))">::at_position<2>);
    static_assert(has_error<R"((?<*))">::at_position<3>);
    static_assert(has_error<R"(ab|+cd)">::at_position<3>);
    static_assert(has_error<R"(((ab))">::at_position<5>);
    static_assert(has_error<R"((?xx))">::at_position<2>);
    static_assert(has_error<R"(a{3x})">::at_position<3>);
    static_assert(has_error<R"(a{34,x})">::at_position<5>);
    static_assert(has_error<R"(a{3,)">::at_position<4>);
    static_assert(has_error<R"(a{1,2,3})">::at_position<5>);
    static_assert(has_error<R"(a**b)">::at_position<2>);
    static_assert(has_error<R"((a(b(c(d))))))">::at_position<12>);
    static_assert(has_error<R"((?<1name>x))">::at_position<3>);
    static_assert(has_error<R"((?<name+>x))">::at_position<7>);
    static_assert(has_error<R"((?<namex))">::at_position<8>);
    static_assert(has_error<R"(\k<)">::at_position<3>);
    static_assert(has_error<R"(\k<name)">::at_position<7>);
    static_assert(has_error<R"(\kname>)">::at_position<2>);
}