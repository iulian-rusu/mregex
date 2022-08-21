#include <mregex.hpp>

namespace meta::tests
{
    using namespace ast;
    using namespace xpr::operators;

    namespace
    {
        template<typename T>
        struct type_wrapper
        {
            template<typename U>
            constexpr bool operator==(type_wrapper<U>) const noexcept
            {
                return std::is_same_v<T, U>;
            }
        };

        template<typename T>
        inline constexpr auto type = type_wrapper<T>{};

        template<typename AST>
        constexpr auto ast_of(regex_interface<AST>) noexcept -> type_wrapper<AST> { return {}; }
    }

    static_assert(ast_of(xpr::nothing) == type<nothing>);
    static_assert(ast_of(xpr::chr<'a'>) == type<literal<'a'>>);
    static_assert(ast_of(xpr::concat(xpr::chr<'a'>)) == type<literal<'a'>>);
    static_assert(
        ast_of(xpr::chr<'a'> >> xpr::chr<'b'>) ==
        type
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
        type
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
        type
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
        ast_of(xpr::str<"abc">) ==
        type
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
        ast_of(not xpr::word >> xpr::str<"abc">) ==
        type
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
    static_assert(
        ast_of(xpr::str<"ab"> >> xpr::zero_or_more(!xpr::digit) >> xpr::str<"xy">) ==
        type
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
    static_assert(
        ast_of(xpr::regex<"ab?c+"> >> xpr::regex<".*">) ==
        type
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
    static_assert(
        ast_of(xpr::chr<'a'> | xpr::chr<'b'>) ==
        type
        <
            alternation
            <
                literal<'a'>,
                literal<'b'>
            >
        >
    );
    static_assert(
        ast_of(xpr::chr<'a'> | xpr::chr<'b'> | xpr::chr<'c'>) ==
        type
        <
            alternation
            <
                literal<'a'>,
                literal<'b'>,
                literal<'c'>
            >
        >
    );
    static_assert(
        ast_of(xpr::either(xpr::chr<'a'>, xpr::chr<'b'>, xpr::chr<'c'>)) ==
        type
        <
            alternation
            <
                literal<'a'>,
                literal<'b'>,
                literal<'c'>
            >
        >
    );
    static_assert(
        ast_of(xpr::str<"ab"> >> (xpr::chr<'c'> | xpr::chr<'d'>)) ==
        type
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
    static_assert(
        ast_of(xpr::begin >> xpr::str<"ab"> >> +(xpr::chr<'c'> | xpr::chr<'d'>) >> xpr::end) ==
        type
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
        type
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
        type
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