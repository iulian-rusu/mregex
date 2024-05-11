#ifndef MREGEX_UTILITY_STATIC_STRING_HPP
#define MREGEX_UTILITY_STATIC_STRING_HPP

#include <array>
#include <iosfwd>
#include <string_view>

namespace meta
{
    /**
     * Helper data structure that implements a constexpr string as a literal type.
     *
     * @tparam N    The length of the string (excluding '\0')
     */
    template<std::size_t N>
    struct static_string
    {
        std::array<char, N> data{};

        constexpr static_string(char const (&str)[N + 1]) noexcept
        {
            if constexpr (N > 0)
                std::copy(str, str + N, std::begin(data));
        }

        constexpr auto length() const noexcept -> std::size_t
        {
            return N;
        }

        constexpr auto begin() noexcept
        {
            return std::begin(data);
        }

        constexpr auto begin() const noexcept
        {
            return std::cbegin(data);
        }

        constexpr auto end() noexcept
        {
            return std::end(data);
        }

        constexpr auto end() const noexcept
        {
            return std::cend(data);
        }

        constexpr explicit operator std::string_view() const noexcept
        requires (N > 0)
        {
            return std::string_view(std::cbegin(data), N);
        }

        constexpr explicit operator std::string_view() const noexcept
        requires (N == 0)
        {
            return "";
        }

        constexpr char operator[](std::size_t i) const noexcept
        {
            return data[i];
        }
    };

    template<std::size_t N>
    static_string(char const (&)[N]) -> static_string<N - 1>;

    /**
     * Constructs a static string from a pack of characters.
     *
     * @note The sizeof...(Chars) is technically redundant, but GCC 11.1 cannot deduce the size.
     */
    template<char... Chars>
    inline constexpr static_string make_static_string = static_string<sizeof...(Chars)>{{Chars ...}};
}

template<std::size_t N>
std::ostream &operator<<(std::ostream &os, meta::static_string<N> const &str)
{
    return os << static_cast<std::string_view>(str);
}
#endif //MREGEX_UTILITY_STATIC_STRING_HPP