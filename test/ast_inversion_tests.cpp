#include "tests.hpp"

#ifdef MREGEX_RUN_AST_TESTS
namespace meta::tests
{
    using namespace ast;

    namespace
    {
        template<typename AST, typename Expected>
        inline constexpr bool expect_inversion = std::is_same_v<invert_t<AST>, Expected>;
    }

    static_assert(expect_inversion<nothing, nothing>);
    static_assert(expect_inversion<literal<'a'>, literal<'a'>>);
    static_assert(expect_inversion<plus<literal<'a'>>, plus<literal<'a'>>>);
    static_assert(expect_inversion<capture<1, literal<'a'>>, capture<1, literal<'a'>>>);
    static_assert(expect_inversion<alternation<wildcard, literal<'a'>>, alternation<wildcard, literal<'a'>>>);
    static_assert(expect_inversion<disjunction<wildcard, literal<'a'>>, disjunction<wildcard, literal<'a'>>>);
    static_assert(expect_inversion<
            repetition<symbol::quantifier_value<3>, symbol::quantifier_value<5>, literal<'a'>>,
            repetition<symbol::quantifier_value<3>, symbol::quantifier_value<5>, literal<'a'>>
    >);
    static_assert(expect_inversion<sequence<wildcard, literal<'a'>>, sequence<literal<'a'>, wildcard>>);
    static_assert(expect_inversion<
            sequence<literal<'a'>, literal<'b'>, literal<'c'>>,
            sequence<literal<'c'>, literal<'b'>, literal<'a'>>
    >);
    static_assert(expect_inversion<
            sequence<literal<'a'>, literal<'b'>, literal<'c'>, hexa>,
            sequence<hexa, literal<'c'>, literal<'b'>, literal<'a'>>
    >);
    static_assert(expect_inversion<
            sequence<literal<'a'>, literal<'b'>, sequence<epsilon, literal<'c'>>>,
            sequence<sequence<literal<'c'>, epsilon>, literal<'b'>, literal<'a'>>
    >);
    static_assert(expect_inversion<
            alternation<literal<'a'>, literal<'b'>, sequence<epsilon, literal<'c'>>>,
            alternation<literal<'a'>, literal<'b'>, sequence<literal<'c'>, epsilon>>
    >);
    static_assert(expect_inversion<
            alternation<literal<'a'>, literal<'b'>, positive_lookahead<sequence<epsilon, literal<'c'>>>>,
            alternation<literal<'a'>, literal<'b'>, positive_lookbehind<sequence<epsilon, literal<'c'>>>>
    >);
    static_assert(expect_inversion<
            sequence
            <
                literal<'x'>,
                star
                <
                    sequence
                    <
                        literal<'a'>,
                        negative_lookahead<wildcard>,
                        literal<'b'>
                    >
                >,
                literal<'y'>
            >,
            sequence
            <
                literal<'y'>,
                star
                <
                    sequence
                    <
                        literal<'b'>,
                        negative_lookbehind<wildcard>,
                        literal<'a'>
                    >
                >,
                literal<'x'>
            >
    >);
}
#endif //MREGEX_RUN_AST_TESTS