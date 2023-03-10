#ifndef MREGEX_UTILITY_CHAR_TRAITS_HPP
#define MREGEX_UTILITY_CHAR_TRAITS_HPP

namespace meta
{
    namespace detail
    {
        /**
         * In ASCII encoding, each lowercase letter is offset by 32 from its uppercase equivalent.
         * Setting the fifth bit of an alphabetical character will make it lowercase.
         */
        inline constexpr char lowercase_bit_mask = 0x20;
    }

    constexpr char flip_lowercase_bit(char input) noexcept
    {
        return static_cast<char>(input ^ detail::lowercase_bit_mask);
    }

    constexpr char set_lowercase_bit(char input) noexcept
    {
        return static_cast<char>(input | detail::lowercase_bit_mask);
    }

    constexpr bool is_numeric(char input) noexcept
    {
        return '0' <= input && input <= '9';
    }

    constexpr bool is_lower(char input) noexcept
    {
        return 'a' <= input && input <= 'z';
    }

    constexpr bool is_upper(char input) noexcept
    {
        return 'A' <= input && input <= 'Z';
    }

    constexpr bool is_alpha(char input) noexcept
    {
        return is_lower(set_lowercase_bit(input));
    }

    constexpr bool is_word(char input) noexcept
    {
        return is_alpha(input) || is_numeric(input) || input == '_';
    }

    constexpr char to_lower(char input) noexcept
    {
        return is_upper(input) ? flip_lowercase_bit(input) : input;
    }

    constexpr char to_upper(char input) noexcept
    {
        return is_lower(input) ? flip_lowercase_bit(input) : input;
    }

    constexpr char invert_case(char input) noexcept
    {
        return is_alpha(input) ? flip_lowercase_bit(input) : input;
    }
}
#endif //MREGEX_UTILITY_CHAR_TRAITS_HPP