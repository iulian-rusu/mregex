#include <mregex.hpp>

namespace meta::tests
{
    namespace
    {
        template<static_string Pattern, typename Stack>
        inline constexpr bool expected_name_spec = std::is_same_v<Stack, ast::capture_name_spec_t<ast_of<Pattern>>>;
    }

    using namespace symbol;

    static_assert(expected_name_spec<"", stack<>>);
    static_assert(expected_name_spec<"abc", stack<>>);
    static_assert(expected_name_spec<"a?b+|c*d", stack<>>);
    static_assert(expected_name_spec<"(a)", stack<unnamed>>);
    static_assert(expected_name_spec<"(a){5}", stack<unnamed>>);
    static_assert(expected_name_spec<"(a)+", stack<unnamed>>);
    static_assert(expected_name_spec<"(?<test>a)", stack<name<"test">>>);
    static_assert(expected_name_spec<"(?<test>a)+", stack<name<"test">>>);
    static_assert(expected_name_spec<"(?<test>a){5,10}", stack<name<"test">>>);
    static_assert(expected_name_spec<"a?(b+)c*", stack<unnamed>>);
    static_assert(expected_name_spec<"(a?)(b+)c*", stack<unnamed, unnamed>>);
    static_assert(expected_name_spec<"(a?)(?<x>b+)c*", stack<unnamed, name<"x">>>);
    static_assert(expected_name_spec<"(a?)(?<x>b+b(b))c*", stack<unnamed, name<"x">, unnamed>>);
    static_assert(expected_name_spec<"(?<z>a?)(?<x>b+b(b))c*", stack<name<"z">, name<"x">, unnamed>>);
    static_assert(expected_name_spec<"((?<a>.)(?<b>(.))((.)(?<c>.)(.)))",
        stack<unnamed, name<"a">, name<"b">, unnamed, unnamed, unnamed, name<"c">, unnamed>
    >);
    static_assert(expected_name_spec<"(?<a>.){5}(?<b>.){5,10}(?<c>.){5,}",
            stack<name<"a">, name<"b">, name<"c">>
    >);
}