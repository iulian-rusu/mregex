#include <mregex.hpp>

namespace meta::tests
{
    namespace
    {
        template<static_string Pattern, typename Stack>
        inline constexpr bool expected_name_spec = std::is_same_v<Stack, ast::capture_name_spec_t<ast_of<Pattern>>>;
    }

    using namespace symbol;

    static_assert(expected_name_spec<"", type_sequence<>>);
    static_assert(expected_name_spec<"abc", type_sequence<>>);
    static_assert(expected_name_spec<"a?b+|c*d", type_sequence<>>);
    static_assert(expected_name_spec<"(a)", type_sequence<unnamed>>);
    static_assert(expected_name_spec<"(a){5}", type_sequence<unnamed>>);
    static_assert(expected_name_spec<"(a)+", type_sequence<unnamed>>);
    static_assert(expected_name_spec<"(?<test>a)", type_sequence<name<"test">>>);
    static_assert(expected_name_spec<"(?<test>a)+", type_sequence<name<"test">>>);
    static_assert(expected_name_spec<"(?<test>a){5,10}", type_sequence<name<"test">>>);
    static_assert(expected_name_spec<"a?(b+)c*", type_sequence<unnamed>>);
    static_assert(expected_name_spec<"(a?)(b+)c*", type_sequence<unnamed, unnamed>>);
    static_assert(expected_name_spec<"(a?)(?<x>b+)c*", type_sequence<unnamed, name<"x">>>);
    static_assert(expected_name_spec<"(a?)(?<x>b+b(b))c*", type_sequence<unnamed, name<"x">, unnamed>>);
    static_assert(expected_name_spec<"(?<z>a?)(?<x>b+b(b))c*", type_sequence<name<"z">, name<"x">, unnamed>>);
    static_assert(expected_name_spec<"((?<a>.)(?<b>(.))((.)(?<c>.)(.)))",
        type_sequence<unnamed, name<"a">, name<"b">, unnamed, unnamed, unnamed, name<"c">, unnamed>
    >);
    static_assert(expected_name_spec<"(?<a>.){5}(?<b>.){5,10}(?<c>.){5,}",
        type_sequence<name<"a">, name<"b">, name<"c">>
    >);
}