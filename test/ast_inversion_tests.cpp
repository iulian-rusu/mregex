#include <mregex.hpp>

namespace meta::tests
{
    using namespace ast;

    namespace
    {
        template<typename AST, typename Expected>
        inline constexpr bool is_inversion = std::is_same_v<invert_t<AST>, Expected>;
    }

    static_assert(is_inversion<nothing, nothing>);
    static_assert(is_inversion<literal<'a'>, literal<'a'>>);
    static_assert(
        is_inversion
        <
            plus<literal<'a'>>,
            plus<literal<'a'>>
        >
    );
    static_assert(
        is_inversion
        <
            unnamed_capture<1, literal<'a'>>,
            unnamed_capture<1, literal<'a'>>
        >
    );
    static_assert(
        is_inversion
        <
            alternation<wildcard, literal<'a'>>,
            alternation<wildcard, literal<'a'>>
        >
    );
    static_assert(
        is_inversion
        <
            repetition<symbol::quantifier_value<3>, symbol::quantifier_value<5>, literal<'a'>>,
            repetition<symbol::quantifier_value<3>, symbol::quantifier_value<5>, literal<'a'>>
        >
    );
    static_assert(
        is_inversion
        <
            sequence<wildcard, literal<'a'>>,
            sequence<literal<'a'>, wildcard>
        >
    );
    static_assert(
        is_inversion
        <
            sequence<literal<'a'>, star<literal<'b'>>, literal<'c'>>,
            sequence<literal<'c'>, star<literal<'b'>>, literal<'a'>>
        >
    );
    static_assert(
        is_inversion
        <
            sequence<literal<'a'>, literal<'b'>, literal<'c'>, literal<'d'>>,
            sequence<literal<'d'>, literal<'c'>, literal<'b'>, literal<'a'>>
        >
    );
    static_assert(
        is_inversion
        <
            sequence<literal<'a'>, literal<'b'>, sequence<empty, literal<'c'>>>,
            sequence<sequence<literal<'c'>, empty>, literal<'b'>, literal<'a'>>
        >
    );
    static_assert(
        is_inversion
        <
            alternation<literal<'a'>, literal<'b'>, sequence<empty, literal<'c'>>>,
            alternation<literal<'a'>, literal<'b'>, sequence<literal<'c'>, empty>>
        >
    );
    static_assert(
        is_inversion
        <
            alternation<literal<'a'>, literal<'b'>, positive_lookahead<sequence<empty, literal<'c'>>>>,
            alternation<literal<'a'>, literal<'b'>, positive_lookbehind<sequence<empty, literal<'c'>>>>
        >
    );
    static_assert(
        is_inversion
        <
            sequence
            <
                atomic<literal<'x'>>,
                optional
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
                optional
                <
                    sequence
                    <
                        literal<'b'>,
                        negative_lookahead<wildcard>,
                        literal<'a'>
                    >
                >,
                atomic<literal<'x'>>
            >
        >
    );
}