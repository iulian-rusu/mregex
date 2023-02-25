#include <mregex.hpp>

namespace meta::tests
{
    using namespace ast;
    using namespace xpr::operators;

    namespace
    {
        template<typename Regex, typename Expected>
        inline constexpr bool expect_ast = std::is_same_v<meta::regex_ast_t<Regex>, Expected>;
    }

    static_assert(expect_ast<decltype(xpr::nothing), nothing>);
    static_assert(expect_ast<decltype(xpr::chr<'a'>), literal<'a'>>);
    static_assert(expect_ast<decltype(xpr::concat(xpr::chr<'a'>)), literal<'a'>>);
    static_assert(
        expect_ast
        <
            decltype(xpr::chr<'a'> >> xpr::chr<'b'>),
            sequence
            <
              literal<'a'>,
              literal<'b'>
            >
        >
    );
    static_assert(
        expect_ast
        <
            decltype(xpr::chr<'a'> >> xpr::chr<'b'> >> xpr::chr<'c'>),
            sequence
            <
                literal<'a'>,
                literal<'b'>,
                literal<'c'>
            >
        >
    );
    static_assert(
        expect_ast
        <
            decltype(xpr::concat(xpr::chr<'a'>,xpr::chr<'b'>,xpr::chr<'c'>)),
            sequence
            <
                literal<'a'>,
                literal<'b'>,
                literal<'c'>
            >
        >
    );
    static_assert(
        expect_ast
        <
            decltype(
                xpr::concat(
                    xpr::chr<'a'>,
                    xpr::str<"bcd">,
                    xpr::chr<'e'>,
                    xpr::wildcard >> xpr::word,
                    xpr::lower | xpr::upper,
                    xpr::concat(
                        xpr::chr<'f'>,
                        xpr::chr<'g'>
                    ),
                    xpr::regex<"hi">
                )
            ),
            sequence
            <
                literal<'a'>,
                literal<'b'>,
                literal<'c'>,
                literal<'d'>,
                literal<'e'>,
                wildcard,
                word,
                alternation<lower, upper>,
                literal<'f'>,
                literal<'g'>,
                literal<'h'>,
                literal<'i'>
            >
        >
    );
    static_assert(
        expect_ast
        <
            decltype(xpr::str<"abc">),
            sequence
            <
                literal<'a'>,
                literal<'b'>,
                literal<'c'>
            >
        >
    );
    static_assert(
        expect_ast
        <
            decltype(not xpr::word >> xpr::str<"abc">),
            sequence
            <
                negated<word>,
                literal<'a'>,
                literal<'b'>,
                literal<'c'>
            >
        >
    );
    static_assert(
        expect_ast
        <
            decltype(xpr::str<"ab"> >> xpr::zero_or_more(!xpr::digit) >> xpr::str<"xy">),
            sequence
            <
                literal<'a'>,
                literal<'b'>,
                star<negated<digit>>,
                literal<'x'>,
                literal<'y'>
            >
        >
    );
    static_assert(
        expect_ast
        <
            decltype(xpr::regex<"ab?c+"> >> xpr::regex<".*">),
            sequence
            <
                literal<'a'>,
                optional<literal<'b'>>,
                plus<literal<'c'>>,
                star<wildcard>
            >
        >
    );
    static_assert(
        expect_ast
        <
            decltype(xpr::chr<'a'> | xpr::chr<'b'>),
            alternation
            <
                literal<'a'>,
                literal<'b'>
            >
        >
    );
    static_assert(
        expect_ast
        <
            decltype(xpr::chr<'a'> | xpr::chr<'b'> | xpr::chr<'c'>),
            alternation
            <
                literal<'a'>,
                literal<'b'>,
                literal<'c'>
            >
        >
    );
    static_assert(
        expect_ast
        <
            decltype(xpr::either(xpr::chr<'a'>, xpr::chr<'b'>, xpr::chr<'c'>)),
            alternation
            <
                literal<'a'>,
                literal<'b'>,
                literal<'c'>
            >
        >
    );
    static_assert(
        expect_ast
        <
            decltype(xpr::str<"ab"> >> (xpr::chr<'c'> | xpr::chr<'d'>)),
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
        >
    );
    static_assert(
        expect_ast
        <
            decltype(xpr::begin_line >> xpr::str<"ab"> >> +(xpr::chr<'c'> | xpr::chr<'d'>) >> xpr::end_line),
            sequence
            <
                beginning_of_line,
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
                end_of_line
            >
        >
    );
    static_assert(
        expect_ast
        <
            decltype(
                xpr::exactly<5>(
                    xpr::either(
                        xpr::begin_line,
                        xpr::chr<'a'> >> xpr::maybe(xpr::chr<'b'>),
                        *xpr::str<"cd">,
                        xpr::end_line
                    )
                )
            ),
            fixed_repetition
            <
                5,
                alternation
                <
                    beginning_of_line,
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
                    end_of_line
                >
            >
        >
    );
    static_assert(
        expect_ast
        <
            decltype(
                xpr::concat(
                    xpr::ahead(xpr::one_or_more<match_mode::lazy>(xpr::word)),
                    xpr::capture<1, "test">(xpr::wildcard),
                    xpr::capture<2>(xpr::word),
                    xpr::negate(xpr::behind(xpr::regex<"ab{2}c">))
                )
            ),
            sequence
            <
                positive_lookahead<lazy_plus<word>>,
                capture
                <
                    1,
                    symbol::name<"test">,
                    wildcard
                >,
                unnamed_capture<2, word>,
                negative_lookbehind
                <
                    sequence
                    <
                        literal<'a'>,
                        fixed_repetition<2, literal<'b'>>,
                        literal<'c'>
                    >
                >
            >
        >
    );
}