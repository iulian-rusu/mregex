#include <mregex.hpp>

namespace meta::tests
{
    using namespace ast;

    namespace
    {
        template<typename AST, typename Expected>
        inline constexpr bool expected_inversion = std::is_same_v<invert_t<AST>, Expected>;
    }

    static_assert(expected_inversion<nothing, nothing>);
    static_assert(expected_inversion<literal<'a'>, literal<'a'>>);
    static_assert(expected_inversion<
        plus<literal<'a'>>,
        plus<literal<'a'>>
    >);
    static_assert(expected_inversion<
        unnamed_capture<1, literal<'a'>>,
        unnamed_capture<1, literal<'a'>>
    >);
    static_assert(expected_inversion<
        alternation<wildcard, literal<'a'>>,
        alternation<wildcard, literal<'a'>>
    >);
    static_assert(expected_inversion<
        repetition<symbol::quantifier_value<3>, symbol::quantifier_value<5>, literal<'a'>>,
        repetition<symbol::quantifier_value<3>, symbol::quantifier_value<5>, literal<'a'>>
    >);
    static_assert(expected_inversion<
        sequence<wildcard, literal<'a'>>,
        sequence<literal<'a'>, wildcard>
    >);
    static_assert(expected_inversion<
        sequence<literal<'a'>, literal<'b'>, literal<'c'>>,
        sequence<literal<'c'>, literal<'b'>, literal<'a'>>
    >);
    static_assert(expected_inversion<
        sequence<literal<'a'>, literal<'b'>, literal<'c'>, literal<'d'>>,
        sequence<literal<'d'>, literal<'c'>, literal<'b'>, literal<'a'>>
    >);
    static_assert(expected_inversion<
        sequence<literal<'a'>, literal<'b'>, sequence<empty, literal<'c'>>>,
        sequence<sequence<literal<'c'>, empty>, literal<'b'>, literal<'a'>>
    >);
    static_assert(expected_inversion<
        alternation<literal<'a'>, literal<'b'>, sequence<empty, literal<'c'>>>,
        alternation<literal<'a'>, literal<'b'>, sequence<literal<'c'>, empty>>
    >);
    static_assert(expected_inversion<
        alternation<literal<'a'>, literal<'b'>, positive_lookahead<sequence<empty, literal<'c'>>>>,
        alternation<literal<'a'>, literal<'b'>, positive_lookbehind<sequence<empty, literal<'c'>>>>
    >);
    static_assert(expected_inversion<
        sequence
        <
            literal<'x'>,
            star
            <
                sequence
                <
                    literal<'a'>,
                    negative_lookbehind<wildcard>,
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
                    negative_lookahead<wildcard>,
                    literal<'a'>
                >
            >,
            literal<'x'>
        >
    >);
}