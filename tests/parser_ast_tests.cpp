#include "tests.h"

#ifdef CTR_RUN_PARSER_TESTS
namespace ctr::tests
{
    static_assert(std::is_same_v<typename parser<"">::ast, epsilon>);
    static_assert(std::is_same_v<typename parser<"a">::ast, character<'a'>>);
    static_assert(std::is_same_v<typename parser<"\\a">::ast, alnum>);
    static_assert(std::is_same_v<typename parser<"\\D">::ast, negated<digit>>);
    static_assert(std::is_same_v<typename parser<"(c)">::ast, capturing<character<'c'>>>);
    static_assert(std::is_same_v<typename parser<"c?">::ast, optional<character<'c'>>>);
    static_assert(std::is_same_v<typename parser<"c*">::ast, star<character<'c'>>>);
    static_assert(std::is_same_v<typename parser<"c+">::ast, plus<character<'c'>>>);
    static_assert(std::is_same_v<typename parser<"(\\(+)*">::ast, star<capturing<plus<character<'('>>>>>);
    static_assert(std::is_same_v<typename parser<"(\\++)*">::ast, star<capturing<plus<character<'+'>>>>>);
    static_assert(std::is_same_v<typename parser<"\\\\">::ast, character<'\\'>>);
    static_assert(std::is_same_v<typename parser<"\\(?x+">::ast,
            sequence<optional<character<'('>>, plus<character<'x'>>>>);
    static_assert(std::is_same_v<typename parser<"abc">::ast,
            sequence<character<'a'>, character<'b'>, character<'c'>>>);
    static_assert(std::is_same_v<typename parser<"a|b|c">::ast,
            alternation<character<'a'>, character<'b'>, character<'c'>>>);
    static_assert(std::is_same_v<typename parser<"aa|bb|cc">::ast,
            alternation<sequence<character<'a'>, character<'a'>>,
                    sequence<character<'b'>, character<'b'>>,
                    sequence<character<'c'>, character<'c'>>>>);
    // make_alternation<make_optional<a>, b> simplifies to make_alternation<a, epsilon, b>
    static_assert(std::is_same_v<typename parser<"a?|b|c">::ast,
            alternation<character<'a'>, epsilon, character<'b'>, character<'c'>>>);
    // slightly more complex AST example
    static_assert(std::is_same_v<typename ctr::parser<"((tuv)?b+)*|xy">::ast,
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
