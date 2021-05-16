#include "tests.h"

#ifdef CTR_RUN_PARSER_TESTS
namespace ctr::tests
{
    template<static_string const pattern, typename AST>
    constexpr bool expected_ast = std::is_same_v<typename parser<pattern>::ast, AST>;

    static_assert(expected_ast<"", epsilon>);
    static_assert(expected_ast<"a", character<'a'>>);
    static_assert(expected_ast<"\\a", alnum>);
    static_assert(expected_ast<"\\D", negated<digit>>);
    static_assert(expected_ast<"(c)", capturing<character<'c'>>>);
    static_assert(expected_ast<"c?", optional<character<'c'>>>);
    static_assert(expected_ast<"c*", star<character<'c'>>>);
    static_assert(expected_ast<"c+", plus<character<'c'>>>);
    static_assert(expected_ast<"(\\(+)*", star<capturing<plus<character<'('>>>>>);
    static_assert(expected_ast<"(\\++)*", star<capturing<plus<character<'+'>>>>>);
    static_assert(expected_ast<"\\\\", character<'\\'>>);
    static_assert(expected_ast<"\\(?x+", sequence<optional<character<'('>>, plus<character<'x'>>>>);
    static_assert(expected_ast<"abc", sequence<character<'a'>, character<'b'>, character<'c'>>>);
    static_assert(expected_ast<"a|b|c", alternation<character<'a'>, character<'b'>, character<'c'>>>);
    static_assert(expected_ast<"aa|bb|cc",
            alternation<sequence<character<'a'>, character<'a'>>,
                    sequence<character<'b'>, character<'b'>>,
                    sequence<character<'c'>, character<'c'>>>>);
    // make_alternation<make_optional<a>, b> simplifies to make_alternation<a, epsilon, b>
    static_assert(expected_ast<"a?|b|c", alternation<character<'a'>, epsilon, character<'b'>, character<'c'>>>);
    // slightly more complex AST example
    static_assert(expected_ast<"((tuv)?b+)*|xy",
            alternation
            <
                star
                <
                    capturing
                    <
                        sequence
                        <
                            optional
                            <
                                capturing
                                <
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
}
#endif // CTR_RUN_PARSER_TESTS
