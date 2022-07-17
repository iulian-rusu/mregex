#ifndef MREGEX_CHAR_TRAITS_HPP
#define MREGEX_CHAR_TRAITS_HPP

namespace meta
{
    template<char C>
    inline constexpr bool is_numeric_v = '0' <= C && C <= '9';

    template<char C>
    inline constexpr bool is_lower_v = 'a' <= C && C <= 'z';

    template<char C>
    inline constexpr bool is_upper_v = 'A' <= C && C <= 'Z';

    template<char C>
    inline constexpr bool is_word = is_lower_v<C> || is_upper_v<C> || is_numeric_v<C> || C == '_';

    template<char C>
    inline constexpr char to_lower_v = is_upper_v<C> ? C ^ 0x20 : C;

    template<char C>
    inline constexpr char to_upper_v = is_lower_v<C> ? C ^ 0x20 : C;

    template<char C>
    inline constexpr char toggle_case_v = (is_lower_v<C> || is_upper_v<C>) ? C ^ 0x20 : C;

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

    constexpr bool is_alpha(char c) noexcept
    {
        return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
    }
}
#endif //MREGEX_CHAR_TRAITS_HPP