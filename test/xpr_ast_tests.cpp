#include <mregex.hpp>

namespace meta::tests
{
    using namespace ast;
    using namespace xpr::operators;

    namespace
    {
        template<typename Expected>
        struct expect
        {
            template<typename AST>
            static constexpr bool equal(regex_interface<AST>) noexcept
            {
                return std::is_same_v<Expected, AST>;
            }
        };
    }

    static_assert(expect<nothing>::equal(xpr::nothing));
    static_assert(expect<literal<'a'>>::equal(xpr::chr<'a'>));
    static_assert(expect<literal<'a'>>::equal(xpr::concat(xpr::chr<'a'>)));
    static_assert(expect<
        sequence
        <
            literal<'a'>,
            literal<'b'>
        >
    >::equal(xpr::chr<'a'> >> xpr::chr<'b'>));
    static_assert(expect<
        sequence
        <
            literal<'a'>,
            literal<'b'>,
            literal<'c'>
        >
    >::equal(xpr::chr<'a'> >> xpr::chr<'b'> >> xpr::chr<'c'>));
    static_assert(expect<
        sequence
        <
            literal<'a'>,
            literal<'b'>,
            literal<'c'>
        >
    >::equal(xpr::concat(xpr::chr<'a'>, xpr::chr<'b'>, xpr::chr<'c'>)));
    static_assert(expect<
        sequence
        <
            literal<'a'>,
            literal<'b'>,
            literal<'c'>
        >
    >::equal(xpr::str<"abc">));
    static_assert(expect<
        sequence
        <
            beginning,
            literal<'a'>,
            literal<'b'>,
            literal<'c'>
        >
    >::equal(xpr::begin >> xpr::str<"abc">));
    static_assert(expect<
        alternation
        <
            literal<'a'>,
            literal<'b'>
        >
    >::equal(xpr::chr<'a'> | xpr::chr<'b'>));
    static_assert(expect<
        alternation
        <
            literal<'a'>,
            literal<'b'>,
            literal<'c'>
        >
    >::equal(xpr::chr<'a'> | xpr::chr<'b'> | xpr::chr<'c'>));
    static_assert(expect<
        alternation
        <
            literal<'a'>,
            literal<'b'>,
            literal<'c'>
        >
    >::equal(xpr::either(xpr::chr<'a'>, xpr::chr<'b'>, xpr::chr<'c'>)));
    static_assert(expect<
        sequence
        <
            literal<'a'>,
            literal<'b'>,
            alternation
            <
                literal<'c'>,
                literal<'d'>
            >
        >
    >::equal(xpr::str<"ab"> >> (xpr::chr<'c'> | xpr::chr<'d'>)));
    static_assert(expect<
        sequence
        <
            beginning,
            literal<'a'>,
            literal<'b'>,
            plus
            <
                alternation
                <
                    literal<'c'>,
                    literal<'d'>
                >
            >,
            end
        >
    >::equal(xpr::begin >> xpr::str<"ab"> >> +(xpr::chr<'c'> | xpr::chr<'d'>) >> xpr::end));
    static_assert(expect<
        fixed_repetition
        <
            5,
            alternation
            <
                beginning,
                sequence
                <
                    literal<'a'>,
                    optional<literal<'b'>>
                >,
                star
                <
                    sequence
                    <
                        literal<'c'>,
                        literal<'d'>
                    >
                >,
                end
            >
        >
    >::equal(
        xpr::exactly<5>(
            xpr::either(
                xpr::begin,
                (xpr::chr<'a'> >> xpr::maybe(xpr::chr<'b'>)),
                *(xpr::str<"cd">),
                xpr::end
            )
        )
    ));
    static_assert(expect<
        sequence
        <
            positive_lookahead
            <
                plus<word>
            >,
            wildcard,
            negative_lookbehind
            <
                sequence
                <
                    literal<'a'>,
                    literal<'b'>,
                    literal<'c'>
                >
            >
        >
    >::equal(
        xpr::concat(
            xpr::ahead(xpr::one_or_more(xpr::word)),
            xpr::any,
            not xpr::behind(xpr::str<"abc">)
        )
    ));
}