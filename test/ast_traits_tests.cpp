#include <mregex.hpp>

namespace meta::tests
{
    using namespace ast;
    using symbol::name;
    using symbol::unnamed;

    namespace
    {
        template<static_string Pattern, typename Expected>
        inline constexpr bool is_name_spec_of = std::is_same_v<capture_name_spec_t<ast_of<Pattern>>, Expected>;
    }

    // Nodes which are trivially matchable by definition
    static_assert(is_trivially_matchable<nothing>);
    static_assert(is_trivially_matchable<negated<nothing>>);
    static_assert(is_trivially_matchable<literal<'a'>>);
    static_assert(is_trivially_matchable<whitespace>);
    static_assert(is_trivially_matchable<wildcard>);
    static_assert(is_trivially_matchable<range<'a', 'z'>>);
    static_assert(is_trivially_matchable<set<wildcard, literal<'b'>, whitespace>>);
    // Nodes which should be deduced as trivially matchable
    static_assert(is_trivially_matchable<sequence<wildcard>>);
    static_assert(is_trivially_matchable<alternation<wildcard>>);
    static_assert(is_trivially_matchable<alternation<whitespace, literal<'a'>, literal<'b'>>>);
    // Nodes which should not be deduced as trivially matchable
    static_assert(is_trivially_matchable<beginning_of_line> == false);
    static_assert(is_trivially_matchable<end_of_input> == false);
    static_assert(is_trivially_matchable<sequence<wildcard, whitespace>> == false);
    static_assert(is_trivially_matchable<alternation<wildcard, beginning_of_line>> == false);
    static_assert(is_trivially_matchable<capture<1, name<"a">, wildcard>> == false);
    static_assert(is_trivially_matchable<backref<1>> == false);
    static_assert(is_trivially_matchable<named_backref<name<"a">>> == false);
    static_assert(is_trivially_matchable<star<literal<'a'>>> == false);
    static_assert(is_trivially_matchable<alternation<star<whitespace>, literal<'a'>, literal<'b'>>> == false);

    static_assert(is_name_spec_of<"", type_sequence<>>);
    static_assert(is_name_spec_of<"abc", type_sequence<>>);
    static_assert(is_name_spec_of<"a?b+|c*d", type_sequence<>>);
    static_assert(is_name_spec_of<"(a)", type_sequence<unnamed>>);
    static_assert(is_name_spec_of<"(a){5}", type_sequence<unnamed>>);
    static_assert(is_name_spec_of<"(a)+", type_sequence<unnamed>>);
    static_assert(is_name_spec_of<"a?(b+)c*", type_sequence<unnamed>>);
    static_assert(is_name_spec_of<"(a?)(b+(c{1,2}))c*", type_sequence<unnamed, unnamed, unnamed>>);
    static_assert(is_name_spec_of<"(?<test>a)", type_sequence<name<"test">>>);
    static_assert(is_name_spec_of<"(?<test>a)+", type_sequence<name<"test">>>);
    static_assert(is_name_spec_of<"(?<test>a){5,10}", type_sequence<name<"test">>>);
    static_assert(is_name_spec_of<"(a?)(?<test>b+)c*", type_sequence<unnamed, name<"test">>>);
    static_assert(is_name_spec_of<"(a?)(?<test>b+b(b))c*", type_sequence<unnamed, name<"test">, unnamed>>);
    static_assert(is_name_spec_of<"(?=(?<ahead>a)+)(?<!(?<behind>a)+)", type_sequence<name<"ahead">, name<"behind">>>);
    static_assert(is_name_spec_of<"(?<a1>a?)(?<a2>b+b(b))c*", type_sequence<name<"a1">, name<"a2">, unnamed>>);
    static_assert(
        is_name_spec_of
        <
            "((?<a1>.)(?<a2>(.))((?<a3>.(?<a4>(?<a5>.).))(.)))",
            type_sequence<unnamed, name<"a1">, name<"a2">, unnamed, unnamed, name<"a3">, name<"a4">, name<"a5">, unnamed>
        >
    );
    static_assert(
        is_name_spec_of
        <
            "(?<a1>.){5}(?<a2>.){5,10}?(?<a3>.){5,}+",
            type_sequence<name<"a1">, name<"a2">, name<"a3">>
        >
    );

    // Nodes which are zero length matchers by definition
    static_assert(is_zero_length_matcher<empty>);
    static_assert(is_zero_length_matcher<backref<1>>);
    static_assert(is_zero_length_matcher<named_backref<name<"abc">>>);
    static_assert(is_zero_length_matcher<beginning_of_line>);
    static_assert(is_zero_length_matcher<beginning_of_input>);
    static_assert(is_zero_length_matcher<end_of_line>);
    static_assert(is_zero_length_matcher<end_of_input>);
    static_assert(is_zero_length_matcher<word_boundary>);
    static_assert(is_zero_length_matcher<negated<word_boundary>>);
    static_assert(is_zero_length_matcher<positive_lookahead<wildcard>>);
    static_assert(is_zero_length_matcher<positive_lookbehind<wildcard>>);
    static_assert(is_zero_length_matcher<negative_lookahead<wildcard>>);
    static_assert(is_zero_length_matcher<negative_lookbehind<wildcard>>);
    // Nodes which should be deduced as zero length matchers
    static_assert(is_zero_length_matcher<plus<empty>>);
    static_assert(is_zero_length_matcher<optional<sequence<wildcard, word>>>);
    static_assert(is_zero_length_matcher<plus<optional<sequence<wildcard, word>>>>);
    static_assert(is_zero_length_matcher<sequence<optional<wildcard>, empty>>);
    static_assert(is_zero_length_matcher<optional<alternation<wildcard, word>>>);
    static_assert(is_zero_length_matcher<alternation<optional<wildcard>, word_boundary>>);
    static_assert(is_zero_length_matcher<sequence<unnamed_capture<1, optional<wildcard>>, star<word>>>);
    // Nodes which should not be deduced as zero length matchers
    static_assert(is_zero_length_matcher<literal<'a'>> == false);
    static_assert(is_zero_length_matcher<set<>> == false);
    static_assert(is_zero_length_matcher<negated<word>> == false);
    static_assert(is_zero_length_matcher<sequence<empty, digit>> == false);
    static_assert(is_zero_length_matcher<alternation<empty, literal<'a'>>> == false);
    static_assert(is_zero_length_matcher<unnamed_capture<1, wildcard>> == false);
    static_assert(is_zero_length_matcher<sequence<unnamed_capture<1, optional<wildcard>>, plus<digit>>> == false);
}