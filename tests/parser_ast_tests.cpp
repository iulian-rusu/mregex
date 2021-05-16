#include "tests.h"

#ifdef CTR_RUN_PARSER_TESTS
namespace ctr::tests
{
    template<static_string pattern, typename AST>
    struct is_ast_of
    {
        static constexpr bool value = std::is_same_v<typename parser<static_string<pattern.length>(pattern)>::ast, AST>;
        constexpr explicit operator bool() noexcept { return value; }
    };

    static_assert(is_ast_of<"", epsilon>{});
    static_assert(is_ast_of<"a", character<'a'>>{});
    static_assert(is_ast_of<"\\a", alnum>{});
    static_assert(is_ast_of<"\\D", negated<digit>>{});
    static_assert(is_ast_of<"(c)", capturing<character<'c'>>>{});
    static_assert(is_ast_of<"c?", optional<character<'c'>>>{});
    static_assert(is_ast_of<"c*", star<character<'c'>>>{});
    static_assert(is_ast_of<"c+", plus<character<'c'>>>{});
    static_assert(is_ast_of<"(\\(+)*", star<capturing<plus<character<'('>>>>>{});
    static_assert(is_ast_of<"(\\++)*", star<capturing<plus<character<'+'>>>>>{});
    static_assert(is_ast_of<"\\\\", character<'\\'>>{});
    static_assert(is_ast_of<"\\(?x+", sequence<optional<character<'('>>, plus<character<'x'>>>>{});
    static_assert(is_ast_of<"abc", sequence<character<'a'>, character<'b'>, character<'c'>>>{});
    static_assert(is_ast_of<"a|b|c", alternation<character<'a'>, character<'b'>, character<'c'>>>{});
    static_assert(is_ast_of<"aa|bb|cc",
            alternation<sequence<character<'a'>, character<'a'>>,
                    sequence<character<'b'>, character<'b'>>,
                    sequence<character<'c'>, character<'c'>>>>{});
    // make_alternation<make_optional<a>, b> simplifies to make_alternation<a, epsilon, b>
    static_assert(is_ast_of<"a?|b|c", alternation<character<'a'>, epsilon, character<'b'>, character<'c'>>>{});
    // slightly more complex AST example
    static_assert(is_ast_of<"((tuv)?b+)*|xy",
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
    >{});
}
#endif // CTR_RUN_PARSER_TESTS
