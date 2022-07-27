#include <mregex.hpp>

namespace meta::tests
{
    using namespace ast;

    namespace
    {
        template<static_string Pattern, typename AST>
        inline constexpr bool expected_ast = std::is_same_v<ast_of<Pattern>, AST>;
    }

    static_assert(expected_ast<R"()", empty>);
    static_assert(expected_ast<R"((?:))", empty>);
    static_assert(expected_ast<R"((?:)+)", plus<empty>>);
    static_assert(expected_ast<R"((?:^)+)", plus<beginning>>);
    static_assert(expected_ast<R"((?!))", negative_lookahead<empty>>);
    static_assert(expected_ast<R"(())", unnamed_capture<1, empty>>);
    static_assert(
        expected_ast
        <
            R"((?<group_name>))",
            capture
            <
                1,
                symbol::name<"group_name">,
                empty
            >
        >
    );
    static_assert(expected_ast<R"(a)", literal<'a'>>);
    static_assert(
        expected_ast
        <
            R"(^ab)",
            sequence
            <
                beginning,
                literal<'a'>,
                literal<'b'>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"(ab$)",
            sequence
            <
                literal<'a'>,
                literal<'b'>,
                end
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"(^ab$)",
            sequence
            <
                beginning,
                literal<'a'>,
                literal<'b'>,
                end
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"(x(?=ab)x)",
            sequence
            <
                literal<'x'>,
                positive_lookahead
                <
                    sequence
                    <
                        literal<'a'>,
                        literal<'b'>
                    >
                >,
                literal<'x'>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"(.+(?<=a+)a)",
            sequence
            <
                plus<wildcard>,
                positive_lookbehind
                <
                    plus<literal<'a'>>
                >,
                literal<'a'>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"(.*(?<!a|b))",
            sequence
            <
                star<wildcard>,
                negative_lookbehind
                <
                    alternation
                    <
                        literal<'a'>,
                        literal<'b'>
                    >
                >
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"(x(?!ab){2}x)",
            sequence
            <
                literal<'x'>,
                fixed_repetition
                <
                    2,
                    negative_lookahead
                    <
                        sequence
                        <
                            literal<'a'>,
                            literal<'b'>
                        >
                    >
                >,
                literal<'x'>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"(x(?<=a(?<!c*d?)b)x)",
            sequence
            <
                literal<'x'>,
                positive_lookbehind
                <
                    sequence
                    <
                        literal<'a'>,
                        negative_lookbehind
                        <
                            sequence
                            <
                                star<literal<'c'>>,
                                optional<literal<'d'>>
                            >
                        >,
                        literal<'b'>
                    >
                >,
                literal<'x'>
            >
        >
    );
    static_assert(expected_ast<R"(\n)", literal<'\n'>>);
    static_assert(expected_ast<R"(\r)", literal<'\r'>>);
    static_assert(expected_ast<R"(\t)", literal<'\t'>>);
    static_assert(expected_ast<R"(\f)", literal<'\f'>>);
    static_assert(expected_ast<R"(\v)", literal<'\v'>>);
    static_assert(expected_ast<R"(\R)", linebreak>);
    static_assert(expected_ast<R"(\N)", negated<linebreak>>);
    static_assert(expected_ast<R"(\a)", literal<'\a'>>);
    static_assert(expected_ast<R"(\D)", negated<digit>>);
    static_assert(expected_ast<R"(\b)", word_boundary>);
    static_assert(expected_ast<R"(\B)", negated<word_boundary>>);
    static_assert(expected_ast<R"(\A)", beginning_of_input>);
    static_assert(expected_ast<R"(\Z)", end_of_input>);
    static_assert(
        expected_ast
        <
            R"(a.?b)",
            sequence
            <
                literal<'a'>,
                optional<wildcard>,
                literal<'b'>
            >
        >
    );
    static_assert(expected_ast<R"((c))", unnamed_capture<1, literal<'c'>>>);
    static_assert(expected_ast<R"((?:c))", literal<'c'>>);
    static_assert(expected_ast<R"(\1)", backref<1>>);
    static_assert(expected_ast<R"(\31)", backref<31>>);
    static_assert(expected_ast<R"(\1+)", plus<backref<1>>>);
    static_assert(expected_ast<R"(\42?)", optional<backref<42>>>);
    static_assert(expected_ast<R"(\1+?)", lazy_plus<backref<1>>>);
    static_assert(expected_ast<R"(\1++)", possessive_plus<backref<1>>>);
    static_assert(expected_ast<R"(\42??)", lazy_optional<backref<42>>>);
    static_assert(expected_ast<R"(\42?+)", possessive_optional<backref<42>>>);
    static_assert(
        expected_ast
        <
            R"((c)\1)",
            sequence
            <
                unnamed_capture<1, literal<'c'>>,
                backref<1>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"((c)\0)",
            sequence
            <
                unnamed_capture<1, literal<'c'>>,
                literal<'\0'>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"((c)\12)",
            sequence
            <
                unnamed_capture<1, literal<'c'>>,
                backref<12>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"((c)\01)",
            sequence
            <
                unnamed_capture<1, literal<'c'>>,
                literal<'\0'>,
                literal<'1'>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"((c)\12?x)",
            sequence
            <
                unnamed_capture<1, literal<'c'>>,
                optional<backref<12>>,
                literal<'x'>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"(((c))(e))",
            sequence
            <
                unnamed_capture
                <
                    1,
                    unnamed_capture<2, literal<'c'>>
                >,
                unnamed_capture<3, literal<'e'>>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"(((?:c))(e))",
            sequence
            <
                unnamed_capture<1, literal<'c'>>,
                unnamed_capture<2, literal<'e'>>
            >
        >
    );
    static_assert(expected_ast<R"(c?)", optional<literal<'c'>>>);
    static_assert(expected_ast<R"(c*)", star<literal<'c'>>>);
    static_assert(expected_ast<R"(c+)", plus<literal<'c'>>>);
    static_assert(expected_ast<R"(c??)", lazy_optional<literal<'c'>>>);
    static_assert(expected_ast<R"(c*?)", lazy_star<literal<'c'>>>);
    static_assert(expected_ast<R"(c+?)", lazy_plus<literal<'c'>>>);
    static_assert(
        expected_ast
        <
            R"(c{2})",
            repetition
            <
                symbol::quantifier_value<2>,
                symbol::quantifier_value<2>,
                literal<'c'>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"(c{2}c{2}?c{2}+)",
            sequence
            <
                fixed_repetition<2, literal<'c'>>,
                lazy_fixed_repetition<2, literal<'c'>>,
                possessive_fixed_repetition<2, literal<'c'>>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"(c{0})",
            repetition
            <
                symbol::quantifier_value<0>,
                symbol::quantifier_value<0>,
                literal<'c'>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"(c{)",
            sequence
            <
                literal<'c'>,
                literal<'{'>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"(c{})",
            sequence
            <
                literal<'c'>,
                literal<'{'>,
                literal<'}'>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"(c{*})",
            sequence
            <
                literal<'c'>,
                star<literal<'{'>>,
                literal<'}'>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"(c{22})",
            repetition
            <
                symbol::quantifier_value<22>,
                symbol::quantifier_value<22>,
                literal<'c'>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"(c{22,})",
            repetition
            <
                symbol::quantifier_value<22>,
                symbol::quantifier_inf,
                literal<'c'>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"(c{22,0}?)", // Parsable syntax, but will not compile when trying to match
            lazy_repetition
            <
                symbol::quantifier_value<22>,
                symbol::quantifier_value<0>,
                literal<'c'>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"(c{984,7644}+)",
            possessive_repetition
            <
                symbol::quantifier_value<984>,
                symbol::quantifier_value<7644>,
                literal<'c'>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"(c{0,22})",
            repetition
            <
                symbol::quantifier_value<0>,
                symbol::quantifier_value<22>,
                literal<'c'>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"(c{,22})",
            sequence
            <
                literal<'c'>,
                literal<'{'>,
                literal<','>,
                literal<'2'>,
                literal<'2'>,
                literal<'}'>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"(c\{22})",
            sequence
            <
                literal<'c'>,
                literal<'{'>,
                literal<'2'>,
                literal<'2'>,
                literal<'}'>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"(c{012})",
            repetition
            <
                symbol::quantifier_value<12>,
                symbol::quantifier_value<12>,
                literal<'c'>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"(c{x})",
            sequence
            <
                literal<'c'>,
                literal<'{'>,
                literal<'x'>,
                literal<'}'>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"(c*\{012})",
            sequence
            <
                star<literal<'c'>>,
                literal<'{'>,
                literal<'0'>,
                literal<'1'>,
                literal<'2'>,
                literal<'}'>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"(\b\{)",
            sequence
            <
                word_boundary,
                literal<'{'>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"(\b\{012})",
            sequence
            <
                word_boundary,
                literal<'{'>,
                literal<'0'>,
                literal<'1'>,
                literal<'2'>,
                literal<'}'>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"(x\A\{+x?)",
            sequence
            <
                literal<'x'>,
                beginning_of_input,
                plus<literal<'{'>>,
                optional<literal<'x'>>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"((c*?\{012}){3})",
            repetition
            <
                symbol::quantifier_value<3>,
                symbol::quantifier_value<3>,
                unnamed_capture
                <
                    1,
                    sequence
                    <
                        lazy_star<literal<'c'>>,
                        literal<'{'>,
                        literal<'0'>,
                        literal<'1'>,
                        literal<'2'>,
                        literal<'}'>
                    >
                >
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"(a(b(c()d)*){15}?)",
            sequence
            <
                literal<'a'>,
                lazy_repetition
                <
                    symbol::quantifier_value<15>,
                    symbol::quantifier_value<15>,
                    unnamed_capture
                    <
                        1,
                        sequence
                        <
                            literal<'b'>,
                            star
                            <
                                unnamed_capture
                                <
                                    2,
                                    sequence
                                    <
                                        literal<'c'>,
                                        unnamed_capture<3, empty>,
                                        literal<'d'>
                                    >
                                >
                            >
                        >
                    >
                >
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"((\(+)*)",
            star
            <
                unnamed_capture
                <
                    1,
                    plus<literal<'('>>
                >
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"((\++)*)",
            star
            <
                unnamed_capture
                <
                    1,
                    plus<literal<'+'>>
                >
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"((?:\++)*?)",
            lazy_star
            <
                plus<literal<'+'>>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"(x(?:abc)*x)",
            sequence
            <
                literal<'x'>,
                star
                <
                    sequence
                    <
                        literal<'a'>,
                        literal<'b'>,
                        literal<'c'>
                    >
                >,
                literal<'x'>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"(x(?:abc)*+x)",
            sequence
            <
                literal<'x'>,
                possessive_star
                <
                    sequence
                    <
                        literal<'a'>,
                        literal<'b'>,
                        literal<'c'>
                    >
                >,
                literal<'x'>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"(()()((()())()))",
            sequence
            <
                unnamed_capture<1, empty>,
                unnamed_capture<2, empty>,
                unnamed_capture
                <
                    3,
                    sequence
                    <
                        unnamed_capture
                        <
                            4,
                            sequence
                            <
                                unnamed_capture<5, empty>,
                                unnamed_capture<6, empty>
                            >
                        >,
                        unnamed_capture<7, empty>
                    >
                >
            >
        >
    );
    static_assert(expected_ast<R"(\\)", literal<'\\'>>);
    static_assert(
        expected_ast
        <
            R"(\(??x+)",
            sequence
            <
                lazy_optional<literal<'('>>,
                plus<literal<'x'>>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"(abc)",
            sequence
            <
                literal<'a'>,
                literal<'b'>,
                literal<'c'>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"(a|b|c)",
            alternation
            <
                literal<'a'>,
                literal<'b'>,
                literal<'c'>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"(aa|bb|cc)",
            alternation
            <
                sequence
                <
                    literal<'a'>,
                    literal<'a'>
                >,
                sequence
                <
                    literal<'b'>,
                    literal<'b'>
                >,
                sequence
                <
                    literal<'c'>,
                    literal<'c'>
                >
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"(a?|b|c)",
            alternation
            <
                optional<literal<'a'>>,
                literal<'b'>,
                literal<'c'>
            >
        >
    );
    static_assert(expected_ast<R"([a])", set<literal<'a'>>>);
    static_assert(expected_ast<R"([.])", set<literal<'.'>>>);
    static_assert(expected_ast<R"([])", nothing>);
    static_assert(expected_ast<R"([^])", negated<nothing>>);
    static_assert(expected_ast<R"([-])", set<literal<'-'>>>);
    static_assert(expected_ast<R"([\0])", set<literal<'\0'>>>);
    static_assert(expected_ast<R"([\1])", set<literal<'1'>>>);
    static_assert(expected_ast<R"([\k])", set<literal<'k'>>>);
    static_assert(
        expected_ast
        <
            R"([\k<test>])",
            set
            <
                literal<'>'>,
                literal<'t'>,
                literal<'s'>,
                literal<'e'>,
                literal<'t'>,
                literal<'<'>,
                literal<'k'>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"([a-])",
            set
            <
                literal<'-'>,
                literal<'a'>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"([\x-])",
            set
            <
                literal<'-'>,
                digit,
                range<'a', 'f'>,
                range<'A', 'F'>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"([\d-a])",
            set
            <
                literal<'a'>,
                literal<'-'>,
                digit
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"([a-\n])",
            set
            <
                range<'a', '\n'>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"([a][b\w])",
            sequence
            <
                set<literal<'a'>>,
                set
                <
                    lower,
                    upper,
                    digit,
                    literal<'_'>,
                    literal<'b'>
                >
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"([][b])",
            sequence
            <
                nothing,
                set<literal<'b'>>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"([a]?[b])",
            sequence
            <
                optional
                <
                    set<literal<'a'>>
                >,
                set<literal<'b'>>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"([a]??[b]?)",
            sequence
            <
                lazy_optional
                <
                    set<literal<'a'>>
                >,
                optional
                <
                    set<literal<'b'>>
                >
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"([abc])",
            set
            <
                literal<'c'>,
                literal<'b'>,
                literal<'a'>
            >
        >
    );
    static_assert(expected_ast<R"([a-z])", set<range<'a', 'z'>>>);
    static_assert(
        expected_ast
        <
            R"([a\-z])",
            set
            <
                literal<'z'>,
                literal<'-'>,
                literal<'a'>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"([a-z-])",
            set
            <
                literal<'-'>,
                range<'a', 'z'>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"([a-z-A])",
            set
            <
                literal<'A'>,
                literal<'-'>,
                range<'a', 'z'>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"([a-z0-9A-Z])",
            set
            <
                range<'A', 'Z'>,
                range<'0', '9'>,
                range<'a', 'z'>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"([a-[0-\]])",
            set
            <
                range<'0', ']'>,
                range<'a', '['>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"([^a-[0-\]])",
            negated
            <
                set
                <
                    range<'0', ']'>,
                    range<'a', '['>
                >
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"([-a-[0-\]])",
            set
            <
                range<'0', ']'>,
                range<'a', '['>,
                literal<'-'>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"([^-aA-Z])",
            negated
            <
                set
                <
                    range<'A', 'Z'>,
                    literal<'a'>,
                    literal<'-'>
                >
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"(a[^-a-z\WA-Z])",
            sequence
            <
                literal<'a'>,
                negated
                <
                    set
                    <
                        range<'A', 'Z'>,
                        negated<word>,
                        range<'a', 'z'>,
                        literal<'-'>
                    >
                >
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"(\h\X[^a^[\]b\c]yy)",
            sequence
            <
                literal<'h'>,
                negated<hexa>,
                negated
                <
                    set
                    <
                        literal<'c'>,
                        literal<'b'>,
                        literal<']'>,
                        literal<'['>,
                        literal<'^'>,
                        literal<'a'>
                    >
                >,
                literal<'y'>,
                literal<'y'>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"(((?<_t3st1ng>tuv)?b+?)*|xy)",
            alternation
            <
                star
                <
                    unnamed_capture
                    <
                        1,
                        sequence
                        <
                            optional
                            <
                                capture
                                <
                                    2,
                                    symbol::name<"_t3st1ng">,
                                    sequence
                                    <
                                        literal<'t'>,
                                        literal<'u'>,
                                        literal<'v'>
                                    >
                                >
                            >,
                            lazy_plus<literal<'b'>>
                        >
                    >
                >,
                sequence
                <
                    literal<'x'>,
                    literal<'y'>
                >
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"((?<test>x+))",
            capture
            <
                1,
                symbol::name<"test">,
                plus<literal<'x'>>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"((?<group_name>x+?)\k<group_name>)",
            sequence
            <
                capture
                <
                    1,
                    symbol::name<"group_name">,
                    lazy_plus<literal<'x'>>
                >,
                named_backref<symbol::name<"group_name">>
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"(x(?<_capture_name>abc)+)",
            sequence
            <
                literal<'x'>,
                plus
                <
                    capture
                    <
                        1,
                        symbol::name<"_capture_name">,
                        sequence
                        <
                            literal<'a'>,
                            literal<'b'>,
                            literal<'c'>
                        >
                    >
                >
            >
        >
    );
    static_assert(
        expected_ast
        <
            R"((?<grp_1>abc)? \k<grp_1> (?<grp_2>xyz){2,})",
            sequence
            <
                optional
                <
                    capture
                    <
                        1,
                        symbol::name<"grp_1">,
                        sequence
                        <
                            literal<'a'>,
                            literal<'b'>,
                            literal<'c'>
                        >
                    >
                >,
                literal<' '>,
                named_backref<symbol::name<"grp_1">>,
                literal<' '>,
                repetition
                <
                    symbol::quantifier_value<2>,
                    symbol::quantifier_inf,
                    capture
                    <
                        2,
                        symbol::name<"grp_2">,
                        sequence
                        <
                            literal<'x'>,
                            literal<'y'>,
                            literal<'z'>
                        >
                    >
                >
            >
        >
    );
}