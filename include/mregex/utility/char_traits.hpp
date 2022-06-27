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
    inline constexpr auto to_lower_v = is_upper_v<C> ? C ^ 0x20 : C;

    template<char C>
    inline constexpr auto to_upper_v = is_lower_v<C> ? C ^ 0x20 : C;

    template<char C>
    inline constexpr auto toggle_case_v = (is_lower_v<C> || is_upper_v<C>) ? C ^ 0x20 : C;

    constexpr auto to_lower(auto c) noexcept
    {
        if ('A' <= c && c <= 'Z')
            return static_cast<decltype(c)>(c ^ 0x20);
        return c;
    }

    constexpr auto to_upper(auto c) noexcept
    {
        if ('a' <= c && c <= 'z')
            return static_cast<decltype(c)>(c ^ 0x20);
        return c;
    }

    constexpr bool is_alpha(auto c) noexcept
    {
        return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
    }
}
#endif //MREGEX_CHAR_TRAITS_HPP