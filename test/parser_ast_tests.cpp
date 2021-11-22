#include "tests.hpp"

#ifdef MREGEX_RUN_PARSER_TESTS
namespace meta::tests
{
    using namespace ast;

    namespace detail
    {
        template<static_string pattern, typename AST>
        inline constexpr bool expected_ast = std::is_same_v<ast_of<pattern>, AST>;
    }

    //static_assert(std::is_same_v<ast_of<static_string(R"(a)")>, literal<'a'>>);

    static_assert(detail::expected_ast<R"()", epsilon>);
    static_assert(detail::expected_ast<R"(a)", literal<'a'>>);
    static_assert(detail::expected_ast<R"(^ab)",
            sequence
            <
                beginning,
                literal<'a'>,
                literal<'b'>
            >
    >);
    static_assert(detail::expected_ast<R"(ab$)",
            sequence
            <
                literal<'a'>,
                literal<'b'>,
                ending
            >
    >);
    static_assert(detail::expected_ast<R"(^ab$)",
            sequence
            <
                beginning,
                literal<'a'>,
                literal<'b'>,
                ending
            >
    >);
    static_assert(detail::expected_ast<R"(x(?=ab)x)",
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
    >);
    static_assert(detail::expected_ast<R"(x(?!ab){2}x)",
            sequence
            <
                literal<'x'>,
                exact_repetition
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
    >);
    static_assert(detail::expected_ast<R"(x(?=a(?!c*d?)b)x)",
            sequence
            <
                literal<'x'>,
                positive_lookahead
                <
                    sequence
                    <
                        literal<'a'>,
                        negative_lookahead
                        <
                            sequence
                            <
                                star
                                <
                                    literal<'c'>
                                >,
                                optional
                                <
                                    literal<'d'>
                                >
                            >
                        >,
                        literal<'b'>
                    >
                >,
                literal<'x'>
            >
    >);
    static_assert(detail::expected_ast<R"(\n)", literal<'\n'>>);
    static_assert(detail::expected_ast<R"(\r)", literal<'\r'>>);
    static_assert(detail::expected_ast<R"(\t)", literal<'\t'>>);
    static_assert(detail::expected_ast<R"(\R)", linebreak>);
    static_assert(detail::expected_ast<R"(\N)", negated<linebreak>>);
    static_assert(detail::expected_ast<R"(\a)", alpha>);
    static_assert(detail::expected_ast<R"(\D)",negated<digit>>);
    static_assert(detail::expected_ast<R"(a.?b)",
            sequence
            <
                literal<'a'>,
                optional
                <
                    wildcard
                >,
                literal<'b'>
            >
    >);
    static_assert(detail::expected_ast<R"((c))",
            capturing
            <
                1,
                literal<'c'>
            >
    >);
    static_assert(detail::expected_ast<R"((?:c))", literal<'c'>>);
    static_assert(detail::expected_ast<R"(\1)", backref<1>>);
    static_assert(detail::expected_ast<R"(\31)", backref<31>>);
    static_assert(detail::expected_ast<R"(\1+)",
            plus
            <
                backref<1>
            >
    >);
    static_assert(detail::expected_ast<R"(\42?)",
            optional
            <
                backref<42>
            >
    >);
    static_assert(detail::expected_ast<R"((c)\1)",
            sequence
            <
                capturing
                <
                    1,
                    literal<'c'>
                >,
                backref<1>
            >
    >);
    static_assert(detail::expected_ast<R"((c)\0)",
            sequence
            <
                capturing
                <
                    1,
                    literal<'c'>
                >,
                literal<'\0'>
            >
    >);
    static_assert(detail::expected_ast<R"((c)\12)",
            sequence
            <
                capturing
                <
                    1,
                    literal<'c'>
                >,
                backref<12>
            >
    >);
    static_assert(detail::expected_ast<R"((c)\01)",
            sequence
            <
                capturing
                <
                    1,
                    literal<'c'>
                >,
                literal<'\0'>,
                literal<'1'>
            >
    >);
    static_assert(detail::expected_ast<R"((c)\12?x)",
            sequence
            <
                capturing
                <
                    1,
                    literal<'c'>
                >,
                optional
                <
                    backref<12>
                >,
                literal<'x'>
            >
    >);
    static_assert(detail::expected_ast<R"(((c))(e))",
            sequence
            <
                capturing
                <
                    1,
                    capturing
                    <
                        2,
                        literal<'c'>
                    >
                >,
                capturing
                <
                    3,
                    literal<'e'>
                >
            >
    >);
    static_assert(detail::expected_ast<R"(((?:c))(e))",
            sequence
            <
                capturing
                <
                    1,
                    literal<'c'>
                >,
                capturing
                <
                    2,
                    literal<'e'>
                >
            >
    >);
    static_assert(detail::expected_ast<R"(c?)",
            optional
            <
                literal<'c'>
            >
    >);
    static_assert(detail::expected_ast<R"(c*)",
            star
            <
                literal<'c'>
            >
    >);
    static_assert(detail::expected_ast<R"(c+)",
            plus
            <
                literal<'c'>
            >
    >);
    static_assert(detail::expected_ast<R"(c{2})",
            repetition
            <
                symbol::quantifier_value<2>,
                symbol::quantifier_value<2>,
                literal<'c'>
            >
    >);
    static_assert(detail::expected_ast<R"(c{0})",
            repetition
            <
                symbol::quantifier_value<0>,
                symbol::quantifier_value<0>,
                literal<'c'>
            >
    >);
    static_assert(detail::expected_ast<R"(c{)",
            sequence
            <
                literal<'c'>,
                literal<'{'>
            >
    >);
    static_assert(detail::expected_ast<R"(c{})",
            sequence
            <
                literal<'c'>,
                literal<'{'>,
                literal<'}'>
            >
    >);
    static_assert(detail::expected_ast<R"(c{*})",
            sequence
            <
                literal<'c'>,
                star
                <
                    literal<'{'>
                >,
                literal<'}'>
            >
    >);
    static_assert(detail::expected_ast<R"(c{22})",
            repetition
            <
                symbol::quantifier_value<22>,
                symbol::quantifier_value<22>,
                literal<'c'>
            >
    >);
    static_assert(detail::expected_ast<R"(c{22,})",
            repetition
            <
                symbol::quantifier_value<22>,
                symbol::quantifier_inf,
                literal<'c'>
            >
    >);
    static_assert(detail::expected_ast<R"(c{22,0})", // parsable syntax but will not compile when trying to match
            repetition
            <
                symbol::quantifier_value<22>,
                symbol::quantifier_value<0>,
                literal<'c'>
            >
    >);
    static_assert(detail::expected_ast<R"(c{984,7644})",
            repetition
            <
                symbol::quantifier_value<984>,
                symbol::quantifier_value<7644>,
                literal<'c'>
            >
    >);
    static_assert(detail::expected_ast<R"(c{0,22})",
            repetition
            <
                symbol::quantifier_value<0>,
                symbol::quantifier_value<22>,
                literal<'c'>
            >
    >);
    static_assert(detail::expected_ast<R"(c{,22})",
            sequence
            <
                literal<'c'>,
                literal<'{'>,
                literal<','>,
                literal<'2'>,
                literal<'2'>,
                literal<'}'>
            >
    >);
    static_assert(detail::expected_ast<R"(c\{22})",
            sequence
            <
                literal<'c'>,
                literal<'{'>,
                literal<'2'>,
                literal<'2'>,
                literal<'}'>
            >
    >);
    static_assert(detail::expected_ast<R"(c{012})",
            repetition
            <
                symbol::quantifier_value<12>,
                symbol::quantifier_value<12>,
                literal<'c'>
            >
    >);
    static_assert(detail::expected_ast<R"(c{x})",
            sequence
            <
                literal<'c'>,
                literal<'{'>,
                literal<'x'>,
                literal<'}'>
            >
    >);
    static_assert(detail::expected_ast<R"(c*{012})",
            sequence
            <
                star
                <
                    literal<'c'>
                >,
                literal<'{'>,
                literal<'0'>,
                literal<'1'>,
                literal<'2'>,
                literal<'}'>
            >
    >);
    static_assert(detail::expected_ast<R"((c*{012}){3})",
            repetition
            <
                symbol::quantifier_value<3>,
                symbol::quantifier_value<3>,
                capturing
                <
                    1,
                    sequence
                    <
                        star
                        <
                            literal<'c'>
                        >,
                        literal<'{'>,
                        literal<'0'>,
                        literal<'1'>,
                        literal<'2'>,
                        literal<'}'>
                    >
                >
            >
    >);
    static_assert(detail::expected_ast<R"(a(b(cd)*){15})",
            sequence
            <
                literal<'a'>,
                repetition
                <
                    symbol::quantifier_value<15>,
                    symbol::quantifier_value<15>,
                    capturing
                    <
                        1,
                        sequence
                        <
                            literal<'b'>,
                            star
                            <
                                capturing
                                <
                                    2,
                                    sequence
                                    <
                                        literal<'c'>,
                                        literal<'d'>
                                    >
                                >
                            >
                        >
                    >
                >
            >
    >);
    static_assert(detail::expected_ast<R"((\(+)*)",
            star
            <
                capturing
                <
                    1,
                    plus
                    <
                        literal<'('>
                    >
                >
            >
    >);
    static_assert(detail::expected_ast<R"((\++)*)",
            star
            <
                capturing
                <
                    1,
                    plus
                    <
                        literal<'+'>
                    >
                >
            >
    >);
    static_assert(detail::expected_ast<R"((?:\++)*)",
            star
            <
                plus
                <
                    literal<'+'>
                >
            >
    >);
    static_assert(detail::expected_ast<R"(x(?:abc)*x)",
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
    >);
    static_assert(detail::expected_ast<R"(xyz?(.(?:a(?:b(c(d))?)*)?)*zyx?)",
            sequence
            <
                literal<'x'>,
                literal<'y'>,
                optional
                <
                    literal<'z'>
                >,
                star
                <
                    capturing
                    <
                        1,
                        sequence
                        <
                            wildcard,
                            optional
                            <
                                sequence
                                <
                                    literal<'a'>,
                                    star
                                    <
                                        sequence
                                        <
                                            literal<'b'>,
                                            optional
                                            <
                                                capturing
                                                <
                                                    2,
                                                    sequence
                                                    <
                                                        literal<'c'>,
                                                        capturing
                                                        <
                                                            3,
                                                            literal<'d'>
                                                        >
                                                    >
                                                >
                                            >
                                        >
                                    >
                                >
                            >
                        >
                    >
                >,
                literal<'z'>,
                literal<'y'>,
                optional
                <
                    literal<'x'>
                >
            >
    >);
    static_assert(detail::expected_ast<R"(\\)", literal<'\\'>>);
    static_assert(detail::expected_ast<R"(\(?x+)",
            sequence
            <
                optional
                <
                    literal<'('>
                >,
                literal<'x'>,
                star
                <
                    literal<'x'>
                >
            >
    >);
    static_assert(detail::expected_ast<R"(abc)",
            sequence
            <
                literal<'a'>,
                literal<'b'>,
                literal<'c'>
            >
    >);
    static_assert(detail::expected_ast<R"(a|b|c)",
            alternation
            <
                literal<'a'>,
                literal<'b'>,
                literal<'c'>
            >
    >);
    static_assert(detail::expected_ast<R"(aa|bb|cc)",
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
    >);
    // alternation<optional<a>, b> simplifies to alternation<a, epsilon, b>
    static_assert(detail::expected_ast<R"(a?|b|c)",
            alternation
            <
                literal<'a'>,
                epsilon,
                literal<'b'>,
                literal<'c'>
            >
    >);
    static_assert(detail::expected_ast<R"([a])",
            set
            <
                literal<'a'>
            >
    >);
    static_assert(detail::expected_ast<R"([.])",
            set
            <
                literal<'.'>
            >
    >);
    static_assert(detail::expected_ast<R"([])", nothing>);
    static_assert(detail::expected_ast<R"([^])", negated<nothing>>);
    static_assert(detail::expected_ast<R"([a][b])",
            sequence
            <
                set
                <
                    literal<'a'>
                >,
                set
                <
                    literal<'b'>
                >
            >
    >);
    static_assert(detail::expected_ast<R"([][b])",
            sequence
            <
                nothing,
                set
                <
                    literal<'b'>
                >
            >
    >);
    static_assert(detail::expected_ast<R"([a]?[b])",
            sequence
            <
                optional
                <
                    set
                    <
                        literal<'a'>
                    >
                >,
                set
                <
                    literal<'b'>
                >
            >
    >);
    static_assert(detail::expected_ast<R"([a]?[b]?)",
            sequence
            <
                optional
                <
                    set
                    <
                        literal<'a'>
                    >
                >,
                optional
                <
                    set
                    <
                        literal<'b'>
                    >
                >
            >
    >);
    static_assert(detail::expected_ast<R"([abc])",
            set
            <
                literal<'c'>,
                literal<'b'>,
                literal<'a'>
            >
    >);
    static_assert(detail::expected_ast<R"([a-z])",
            set
            <
                range<'a', 'z'>
            >
    >);
    static_assert(detail::expected_ast<R"([a\-z])",
            set
            <
                literal<'z'>,
                literal<'-'>,
                literal<'a'>
            >
    >);
    static_assert(detail::expected_ast<R"([a-z-])",
            set
            <
                literal<'-'>,
                range<'a', 'z'>
            >
    >);
    static_assert(detail::expected_ast<R"([a-z-A])",
            set
            <
                literal<'A'>,
                literal<'-'>,
                range<'a', 'z'>
            >
    >);
    static_assert(detail::expected_ast<R"([a-z0-9A-Z])",
            set
            <
                range<'A', 'Z'>,
                range<'0', '9'>,
                range<'a', 'z'>
            >
    >);
    static_assert(detail::expected_ast<R"([a-[0-\]])",
            set
            <
                range<'0', ']'>,
                range<'a', '['>
            >
    >);
    static_assert(detail::expected_ast<R"([^a-[0-\]])",
            negated
            <
                set
                <
                    range<'0', ']'>,
                    range<'a', '['>
                >
            >
    >);
    static_assert(detail::expected_ast<R"([a-[0-\]])",
            set
            <
                range<'0', ']'>,
                range<'a', '['>
            >
    >);
    static_assert(detail::expected_ast<R"([-a-[0-\]])",
            set
            <
                range<'0', ']'>,
                range<'a', '['>,
                literal<'-'>
            >
    >);
    static_assert(detail::expected_ast<R"([^-aA-Z])",
            negated
            <
                set
                <
                    range<'A', 'Z'>,
                    literal<'a'>,
                    literal<'-'>
                >
            >
    >);
    static_assert(detail::expected_ast<R"(a[^-a-z\WA-Z])",
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
    >);
    static_assert(detail::expected_ast<R"(\x\x[^a^[\]b\c]yy)",
            sequence
            <
                literal<'x'>,
                literal<'x'>,
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
    >);
    static_assert(detail::expected_ast<R"(((tuv)?b+)*|xy)",
            alternation
            <
                star
                <
                    capturing
                    <
                        1,
                        sequence
                        <
                            optional
                            <
                                capturing
                                <
                                    2,
                                    sequence
                                    <
                                        literal<'t'>,
                                        literal<'u'>,
                                        literal<'v'>
                                    >
                                >
                            >,
                            literal<'b'>,
                            star
                            <
                                literal<'b'>
                            >
                        >
                    >
                >,
                sequence
                <
                    literal<'x'>,
                    literal<'y'>
                >
            >
    >);
}
#endif //MREGEX_RUN_PARSER_TESTS