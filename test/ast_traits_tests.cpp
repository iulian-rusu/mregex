#include <mregex.hpp>

namespace meta::tests
{
    using namespace ast;
    using namespace symbol;

    namespace
    {
        template<static_string Pattern, typename Sequence>
        inline constexpr bool matching_name_spec = std::is_same_v<capture_name_spec_t<ast_for<Pattern>>, Sequence>;
    }

    static_assert(is_trivially_matchable_v<nothing>);
    static_assert(is_trivially_matchable_v<negated<nothing>>);
    static_assert(is_trivially_matchable_v<literal<'a'>>);
    static_assert(is_trivially_matchable_v<whitespace>);
    static_assert(is_trivially_matchable_v<wildcard>);
    static_assert(is_trivially_matchable_v<range<'a', 'z'>>);
    static_assert(is_trivially_matchable_v<set<wildcard, literal<'b'>, whitespace>>);
    static_assert(is_trivially_matchable_v<sequence<wildcard>>);
    static_assert(is_trivially_matchable_v<alternation<wildcard>>);
    static_assert(is_trivially_matchable_v<alternation<whitespace, literal<'a'>, literal<'b'>>>);
    static_assert(is_trivially_matchable_v<beginning_of_line> == false);
    static_assert(is_trivially_matchable_v<end_of_input> == false);
    static_assert(is_trivially_matchable_v<sequence<wildcard, whitespace>> == false);
    static_assert(is_trivially_matchable_v<alternation<wildcard, beginning_of_line>> == false);
    static_assert(is_trivially_matchable_v<capture<1, symbol::name<"a">, wildcard>> == false);
    static_assert(is_trivially_matchable_v<backref<1>> == false);
    static_assert(is_trivially_matchable_v<named_backref<symbol::name<"a">>> == false);
    static_assert(is_trivially_matchable_v<star<literal<'a'>>> == false);
    static_assert(is_trivially_matchable_v<alternation<star<whitespace>, literal<'a'>, literal<'b'>>> == false);

    static_assert(matching_name_spec<"", type_sequence<>>);
    static_assert(matching_name_spec<"abc", type_sequence<>>);
    static_assert(matching_name_spec<"a?b+|c*d", type_sequence<>>);
    static_assert(matching_name_spec<"(a)", type_sequence<unnamed>>);
    static_assert(matching_name_spec<"(a){5}", type_sequence<unnamed>>);
    static_assert(matching_name_spec<"(a)+", type_sequence<unnamed>>);
    static_assert(matching_name_spec<"(?<test>a)", type_sequence<name<"test">>>);
    static_assert(matching_name_spec<"(?<test>a)+", type_sequence<name<"test">>>);
    static_assert(matching_name_spec<"(?<test>a){5,10}", type_sequence<name<"test">>>);
    static_assert(matching_name_spec<"a?(b+)c*", type_sequence<unnamed>>);
    static_assert(matching_name_spec<"(a?)(b+)c*", type_sequence<unnamed, unnamed>>);
    static_assert(matching_name_spec<"(a?)(?<x>b+)c*", type_sequence<unnamed, name<"x">>>);
    static_assert(matching_name_spec<"(a?)(?<x>b+b(b))c*", type_sequence<unnamed, name<"x">, unnamed>>);
    static_assert(matching_name_spec<"(?<z>a?)(?<x>b+b(b))c*", type_sequence<name<"z">, name<"x">, unnamed>>);
    static_assert(
        matching_name_spec
        <
            "((?<a>.)(?<bb>(.))((.)(?<ccc>.)(.)))",
            type_sequence<unnamed, name<"a">, name<"bb">, unnamed, unnamed, unnamed, name<"ccc">, unnamed>
        >
    );
    static_assert(
        matching_name_spec
        <
            "(?<a>.){5}(?<bb>.){5,10}(?<ccc>.){5,}",
            type_sequence<name<"a">, name<"bb">, name<"ccc">>
        >
    );
}