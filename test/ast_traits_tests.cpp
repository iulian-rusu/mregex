#include <mregex.hpp>

namespace meta::tests
{
    using namespace ast;
    using namespace symbol;

    namespace
    {
        template<static_string Pattern, typename Sequence>
        inline constexpr bool has_name_spec = std::is_same_v<capture_name_spec_t<ast_for<Pattern>>, Sequence>;
    }

    static_assert(is_trivially_matchable<nothing>);
    static_assert(is_trivially_matchable<negated<nothing>>);
    static_assert(is_trivially_matchable<literal<'a'>>);
    static_assert(is_trivially_matchable<whitespace>);
    static_assert(is_trivially_matchable<wildcard>);
    static_assert(is_trivially_matchable<range<'a', 'z'>>);
    static_assert(is_trivially_matchable<set<wildcard, literal<'b'>, whitespace>>);
    static_assert(is_trivially_matchable<sequence<wildcard>>);
    static_assert(is_trivially_matchable<alternation<wildcard>>);
    static_assert(is_trivially_matchable<alternation<whitespace, literal<'a'>, literal<'b'>>>);
    static_assert(is_trivially_matchable<beginning_of_line> == false);
    static_assert(is_trivially_matchable<end_of_input> == false);
    static_assert(is_trivially_matchable<sequence<wildcard, whitespace>> == false);
    static_assert(is_trivially_matchable<alternation<wildcard, beginning_of_line>> == false);
    static_assert(is_trivially_matchable<capture<1, symbol::name<"a">, wildcard>> == false);
    static_assert(is_trivially_matchable<backref<1>> == false);
    static_assert(is_trivially_matchable<named_backref<symbol::name<"a">>> == false);
    static_assert(is_trivially_matchable<star<literal<'a'>>> == false);
    static_assert(is_trivially_matchable<alternation<star<whitespace>, literal<'a'>, literal<'b'>>> == false);

    static_assert(has_name_spec<"", type_sequence<>>);
    static_assert(has_name_spec<"abc", type_sequence<>>);
    static_assert(has_name_spec<"a?b+|c*d", type_sequence<>>);
    static_assert(has_name_spec<"(a)", type_sequence<unnamed>>);
    static_assert(has_name_spec<"(a){5}", type_sequence<unnamed>>);
    static_assert(has_name_spec<"(a)+", type_sequence<unnamed>>);
    static_assert(has_name_spec<"(?<test>a)", type_sequence<name<"test">>>);
    static_assert(has_name_spec<"(?<test>a)+", type_sequence<name<"test">>>);
    static_assert(has_name_spec<"(?<test>a){5,10}", type_sequence<name<"test">>>);
    static_assert(has_name_spec<"a?(b+)c*", type_sequence<unnamed>>);
    static_assert(has_name_spec<"(a?)(b+)c*", type_sequence<unnamed, unnamed>>);
    static_assert(has_name_spec<"(a?)(?<x>b+)c*", type_sequence<unnamed, name<"x">>>);
    static_assert(has_name_spec<"(a?)(?<x>b+b(b))c*", type_sequence<unnamed, name<"x">, unnamed>>);
    static_assert(has_name_spec<"(?<z>a?)(?<x>b+b(b))c*", type_sequence<name<"z">, name<"x">, unnamed>>);
    static_assert(
        has_name_spec
        <
            "((?<a>.)(?<bb>(.))((.)(?<ccc>.)(.)))",
            type_sequence<unnamed, name<"a">, name<"bb">, unnamed, unnamed, unnamed, name<"ccc">, unnamed>
        >
    );
    static_assert(
        has_name_spec
        <
            "(?<a>.){5}(?<bb>.){5,10}(?<ccc>.){5,}",
            type_sequence<name<"a">, name<"bb">, name<"ccc">>
        >
    );
}