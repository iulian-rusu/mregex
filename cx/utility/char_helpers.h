#ifndef CX_CHAR_HELPERS_H
#define CX_CHAR_HELPERS_H

namespace cx
{
    template<auto C>
    constexpr bool is_whitespace_v = C == ' ' || C == '\t' ||
                                     C == '\n' || C == '\r' ||
                                     C == '\f' || C == '\x0B';
    template<auto C>
    constexpr bool is_lower_v = 'a' <= C && C <= 'z';

    template<auto C>
    constexpr bool is_upper_v = 'A' <= C && C <= 'Z';

    template<auto C>
    constexpr auto to_lower_v = is_upper_v<C> ? C ^ 0x20 : C;

    template<auto C>
    constexpr auto to_upper_v = is_lower_v<C> ? C ^ 0x20 : C;

    template<auto C>
    constexpr auto toggle_case_v = is_lower_v<C> || is_upper_v<C> ? C ^ 0x20 : C;

    constexpr auto to_lower(auto c) noexcept
    {
        if ('A' <= c && c <= 'Z')
        {
            return static_cast<decltype(c)>(c ^ 0x20);
        }
        return c;
    }

    constexpr auto to_upper(auto c) noexcept
    {
        if ('a' <= c && c <= 'z')
        {
            return static_cast<decltype(c)>(c ^ 0x20);
        }
        return c;
    }
}
#endif //CX_CHAR_HELPERS_H
