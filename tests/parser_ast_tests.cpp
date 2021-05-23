#include "tests.h"

#ifdef CX_RUN_PARSER_TESTS
namespace cx::tests
{
    template<static_string const pattern, typename AST>
    constexpr bool expected_ast = std::is_same_v<typename parser<pattern>::ast, AST>;

    static_assert(expected_ast<"", epsilon>);
    static_assert(expected_ast<"a", character<'a'>>);
    static_assert(expected_ast<"\\a", alnum>);
    static_assert(expected_ast<"\\D", negated<digit>>);
    static_assert(expected_ast<"a.?b", sequence<character<'a'>, optional<wildcard>, character<'b'>>>);
    static_assert(expected_ast<"(c)", capturing<1, character<'c'>>>);
    static_assert(expected_ast<"((c))(e)", sequence<capturing<1, capturing<2, character<'c'>>>, capturing<3, character<'e'>>>>);
    static_assert(expected_ast<"c?", optional<character<'c'>>>);
    static_assert(expected_ast<"c*", star<character<'c'>>>);
    static_assert(expected_ast<"c+", plus<character<'c'>>>);
    static_assert(expected_ast<"(\\(+)*", star<capturing<1, plus<character<'('>>>>>);
    static_assert(expected_ast<"(\\++)*", star<capturing<1, plus<character<'+'>>>>>);
    static_assert(expected_ast<"\\\\", character<'\\'>>);
    static_assert(expected_ast<"\\(?x+", sequence<optional<character<'('>>, plus<character<'x'>>>>);
    static_assert(expected_ast<"abc", sequence<character<'a'>, character<'b'>, character<'c'>>>);
    static_assert(expected_ast<"a|b|c", alternation<character<'a'>, character<'b'>, character<'c'>>>);
    static_assert(expected_ast<"aa|bb|cc",
            alternation<sequence<character<'a'>, character<'a'>>,
                    sequence<character<'b'>, character<'b'>>,
                    sequence<character<'c'>, character<'c'>>>>);
    // alternation<optional<a>, b> simplifies to alternation<a, epsilon, b>
    static_assert(expected_ast<"a?|b|c", alternation<character<'a'>, epsilon, character<'b'>, character<'c'>>>);
    // AST with sets
    static_assert(expected_ast<R"([a])", alternation<character<'a'>>>);
    static_assert(expected_ast<R"([a][b])", sequence<alternation<character<'a'>>, alternation<character<'b'>>>>);
    static_assert(expected_ast<R"([a]?[b])", sequence<optional<alternation<character<'a'>>>, alternation<character<'b'>>>>);
    static_assert(expected_ast<R"([a]?[b]?)", sequence<optional<alternation<character<'a'>>>, optional<alternation<character<'b'>>>>>);
    static_assert(expected_ast<R"([abc])", alternation<character<'c'>, character<'b'>, character<'a'>>>);
    static_assert(expected_ast<R"([a-z])", alternation<range<'a', 'z'>>>);
    static_assert(expected_ast<R"([a\-z])", alternation<character<'z'>, character<'-'>, character<'a'>>>);
    static_assert(expected_ast<R"([a-z0-9A-Z])", alternation<range<'A', 'Z'>, range<'0', '9'>, range<'a', 'z'>>>);
    static_assert(expected_ast<R"([a-[0-\]])", alternation<range<'0', ']'>, range<'a', '['>>>);
    static_assert(expected_ast<R"([^a-[0-\]])", negated<alternation<range<'0', ']'>, range<'a', '['>>>>);
    static_assert(expected_ast<R"([a-[0-\])", alternation<range<'0', ']'>, range<'a', '['>>>);
    static_assert(expected_ast<R"([-a-[0-\])", alternation<range<'0', ']'>, range<'a', '['>, character<'-'>>>);
    static_assert(expected_ast<R"([^-aA-Z])", negated<alternation<range<'A', 'Z'>, character<'a'>, character<'-'>>>>);
    static_assert(expected_ast<R"(a[^-a-z\WA-Z])",
            sequence<character<'a'>, negated<alternation<range<'A', 'Z'>, negated<word>, range<'a', 'z'>, character<'-'>>>>>);
    static_assert(expected_ast<R"(\x\x[^a^[\]b\c]yy)",
            sequence
            <
                character<'x'>, character<'x'>,
                negated
                <
                    alternation
                    <
                        character<'c'>,
                        character<'b'>,
                        character<']'>,
                        character<'['>,
                        character<'^'>,
                        character<'a'>
                    >
                >,
                character<'y'>, character<'y'>
            >
    >);
    // slightly more complex AST example
    static_assert(expected_ast<"((tuv)?b+)*|xy",
            alternation
            <
                star
                <
                    capturing
                    <   1,
                        sequence
                        <
                            optional
                            <
                                capturing
                                <   2,
                                    sequence
                                    <
                                        character<'t'>,
                                        character<'u'>,
                                        character<'v'>
                                    >
                                >
                            >,
                            plus
                            <
                                character<'b'>
                            >
                        >
                    >
                >,
                sequence
                <
                    character<'x'>,
                    character<'y'>
                >
            >
    >);

    // for debugging
    // static_assert(std::is_same_v<typename parser<static_string(R"([^-aA-Z])")>::ast, void>);
}
#endif // CX_RUN_PARSER_TESTS
