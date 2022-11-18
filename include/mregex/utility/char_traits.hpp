#ifndef MREGEX_UTILITY_CHAR_TRAITS_HPP
#define MREGEX_UTILITY_CHAR_TRAITS_HPP

namespace meta
{
    template<char C>
    inline constexpr bool is_numeric = '0' <= C && C <= '9';

    template<char C>
    inline constexpr bool is_lower = 'a' <= C && C <= 'z';

    template<char C>
    inline constexpr bool is_upper = 'A' <= C && C <= 'Z';

    template<char C>
    inline constexpr bool is_word = is_lower<C> || is_upper<C> || is_numeric<C> || C == '_';

    template<char C>
    inline constexpr char toggle_case = (is_lower<C> || is_upper<C>) ? C ^ 0x20 : C;

    constexpr bool is_alpha(char c) noexcept
    {
        return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
    }

    constexpr char to_lower(char c) noexcept
    {
        if ('A' <= c && c <= 'Z')
            return static_cast<char>(c ^ 0x20);
        return c;
    }

    constexpr char to_upper(char c) noexcept
    {
        if ('a' <= c && c <= 'z')
            return static_cast<char>(c ^ 0x20);;
        return c;
    }
}
#endif //MREGEX_UTILITY_CHAR_TRAITS_HPP