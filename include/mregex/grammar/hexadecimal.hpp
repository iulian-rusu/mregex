#ifndef MREGEX_GRAMMAR_HEXADECIMAL_HPP
#define MREGEX_GRAMMAR_HEXADECIMAL_HPP

#include <mregex/grammar/actions.hpp>
#include <mregex/utility/char_traits.hpp>
#include <mregex/utility/type_sequence.hpp>

namespace meta::grammar
{
    namespace detail
    {
        template<typename Sequence, char C, bool = is_hexdigit(C)>
        struct update_hex_escape_sequence;

        template<std::uint8_t... Hexdigits, char C>
        struct update_hex_escape_sequence<symbol::hex_esc_seq<Hexdigits ...>, C, true>
        {
            static constexpr std::uint8_t digit = is_digit(C) ? C - '0' : 10 + set_lowercase_bit(C) - 'a';

            using type =
                    type_sequence
                    <
                        advance,
                        symbol::hex_esc_seq<Hexdigits ..., digit>
                    >;
        };

        template<typename Sequence, char C>
        struct update_hex_escape_sequence<Sequence, C, false>
        {
            using type = reject;
        };

        template<typename Sequence, char C>
        using update_hex_escape_sequence_t = typename update_hex_escape_sequence<Sequence, C>::type;
    }

    struct begin_hex_escape_sequence
    {
        using type =
                type_sequence
                <
                    advance,
                    symbol::hex_esc_seq<>
                >;
    };

    template<typename Sequence, typename Token>
    struct update_hex_escape_sequence
    {
        using type = reject;
    };

    template<std::uint8_t... Hexdigits, char C>
    requires (sizeof...(Hexdigits) < 2)
    struct update_hex_escape_sequence<symbol::hex_esc_seq<Hexdigits ...>, symbol::character<C>>
    {
        using type = detail::update_hex_escape_sequence_t<symbol::hex_esc_seq<Hexdigits ...>, C>;
    };

    // The escape sequence is finished when it contains 2 hex digits
    template<std::uint8_t First, std::uint8_t Second, typename Token>
    struct update_hex_escape_sequence<symbol::hex_esc_seq<First, Second>, Token>
    {
        using type = symbol::push_literal<static_cast<char>(First * 16 + Second)>;
    };

    template<typename Sequence, typename Token>
    using update_hex_escape_sequence_t = typename update_hex_escape_sequence<Sequence, Token>::type;
}
#endif //MREGEX_GRAMMAR_HEXADECIMAL_HPP
