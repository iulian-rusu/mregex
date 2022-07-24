#include <mregex.hpp>

namespace meta::tests
{
    using namespace ast;
    using namespace xpr::operators;

    namespace
    {
        template<typename T>
        struct comparable
        {
            template<typename U>
            constexpr bool operator==(comparable<U>) const noexcept
            {
                return std::is_same_v<T, U>;
            }
        };

        template<typename AST>
        inline constexpr auto ast_type = comparable<AST>{};

        template<typename AST>
        constexpr auto ast_of(regex_interface<AST>) noexcept -> comparable<AST> { return {}; }
    }

    static_assert(ast_of(xpr::nothing) == ast_type<nothing>);
    static_assert(ast_of(xpr::chr<'a'>) == ast_type<literal<'a'>>);
    static_assert(ast_of(xpr::concat(xpr::chr<'a'>)) == ast_type<literal<'a'>>);
    static_assert(
        ast_of(xpr::chr<'a'> >> xpr::chr<'b'>) ==
        ast_type
        <
            sequence
            <
              literal<'a'>,
              literal<'b'>
            >
        >
    );
    static_assert(
        ast_of(xpr::chr<'a'> >> xpr::chr<'b'> >> xpr::chr<'c'>) ==
        ast_type
        <
            sequence
            <
                literal<'a'>,
                literal<'b'>,
                literal<'c'>
            >
        >
    );
    static_assert(
        ast_of(
            xpr::concat(
                xpr::chr<'a'>,
                xpr::chr<'b'>,
                xpr::chr<'c'>
            )
        ) ==
        ast_type
        <
            sequence
            <
                literal<'a'>,
                literal<'b'>,
                literal<'c'>
            >
        >
    );
    static_assert(ast_of(xpr::str<"abc">) ==
        ast_type
        <
            sequence
            <
                literal<'a'>,
                literal<'b'>,
                literal<'c'>
            >
        >
    );
    static_assert(ast_of(not xpr::word >> xpr::str<"abc">) ==
        ast_type
        <
            sequence
            <
                negated<word>,
                literal<'a'>,
                literal<'b'>,
                literal<'c'>
            >
        >
    );
    static_assert(ast_of(xpr::str<"ab"> >> xpr::zero_or_more(!xpr::digit) >> xpr::str<"xy">) ==
        ast_type
        <
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
    static_assert(ast_of(xpr::regex<"ab?c+"> >> xpr::regex<".*">) ==
        ast_type
        <
            sequence
            <
                literal<'a'>,
                optional<literal<'b'>>,
                plus<literal<'c'>>,
                star<wildcard>
            >
        >
    );
    static_assert(ast_of(xpr::chr<'a'> | xpr::chr<'b'>) ==
        ast_type
        <
            alternation
            <
                literal<'a'>,
                literal<'b'>
            >
        >
    );
    static_assert(ast_of(xpr::chr<'a'> | xpr::chr<'b'> | xpr::chr<'c'>) ==
        ast_type
        <
            alternation
            <
                literal<'a'>,
                literal<'b'>,
                literal<'c'>
            >
        >
    );
    static_assert(ast_of(xpr::either(xpr::chr<'a'>, xpr::chr<'b'>, xpr::chr<'c'>)) ==
        ast_type
        <
            alternation
            <
                literal<'a'>,
                literal<'b'>,
                literal<'c'>
            >
        >
    );
    static_assert(ast_of(xpr::str<"ab"> >> (xpr::chr<'c'> | xpr::chr<'d'>)) ==
        ast_type
        <
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
    static_assert(ast_of(xpr::begin >> xpr::str<"ab"> >> +(xpr::chr<'c'> | xpr::chr<'d'>) >> xpr::end) ==
        ast_type
        <
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
        >
    );
    static_assert(
        ast_of(
            xpr::exactly<5>(
                xpr::either(
                    xpr::begin,
                    (xpr::chr<'a'> >> xpr::maybe(xpr::chr<'b'>)),
                    *(xpr::str<"cd">),
                    xpr::end
                )
            )
        ) ==
        ast_type
        <
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
        >
    );
    static_assert(
        ast_of(
            xpr::concat(
                xpr::ahead(
                    xpr::one_or_more<match_mode::lazy>(xpr::word)
                ),
                xpr::any,
                xpr::negate(
                    xpr::behind(xpr::regex<"ab{2}c">)
                )
            )
        ) ==
        ast_type
        <
            sequence
            <
                positive_lookahead
                <
                    lazy_plus<word>
                >,
                wildcard,
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