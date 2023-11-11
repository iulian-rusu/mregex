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
        inline constexpr std::uint8_t lowercase_bit_mask = 0x20;
    }

    /**
     *  Checks if the given ASCII code is in the specified range (inclusive).
     *  This should be used instead of directly comparing chars since char is a signed type.
     */
    template<std::uint8_t A, std::uint8_t B>
    constexpr bool is_in_ascii_range(std::uint8_t input) noexcept
    {
        return A <= input && input <= B;
    }

    constexpr char flip_lowercase_bit(char input) noexcept
    {
        return static_cast<char>(input ^ detail::lowercase_bit_mask);
    }

    constexpr char set_lowercase_bit(char input) noexcept
    {
        return static_cast<char>(input | detail::lowercase_bit_mask);
    }

    constexpr bool is_digit(char input) noexcept
    {
        return is_in_ascii_range<'0', '9'>(input);
    }

    constexpr bool is_hexdigit(char input) noexcept
    {
        return is_digit(input) || is_in_ascii_range<'a', 'f'>(set_lowercase_bit(input));
    }

    constexpr bool is_lower(char input) noexcept
    {
        return is_in_ascii_range<'a', 'z'>(input);
    }

    constexpr bool is_upper(char input) noexcept
    {
        return is_in_ascii_range<'A', 'Z'>(input);
    }

    constexpr bool is_alpha(char input) noexcept
    {
        return is_lower(set_lowercase_bit(input));
    }

    constexpr bool is_word(char input) noexcept
    {
        return is_alpha(input) || is_digit(input) || input == '_';
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