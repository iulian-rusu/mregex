#ifndef MREGEX_UTILITY_STATIC_STRING_HPP
#define MREGEX_UTILITY_STATIC_STRING_HPP

#include <array>
#include <iosfwd>
#include <string_view>
#include <type_traits>

namespace meta
{
    template<std::size_t N>
    struct static_string_base
    {
        char data[N]{};
    };

    template<>
    struct static_string_base<0>
    {
        char *data = nullptr;
    };

    /**
     * Helper class that implements a constexpr string as a literal type.
     *
     * @tparam N    The length of the string (excluding '\0')
     */
    template<std::size_t N>
    struct static_string : static_string_base<N>
    {
        using static_string_base<N>::data;

        constexpr static_string(char const (&str)[N + 1]) noexcept
        {
            if constexpr (N > 0)
                std::copy(str, str + N, data);
        }

        constexpr static_string(static_string const &other) noexcept
        {
            if constexpr (N > 0)
                std::copy(other.data, other.data + N, data);
        }

        [[nodiscard]] constexpr auto length() const noexcept
        {
            return N;
        }

        [[nodiscard]] constexpr auto begin() noexcept
        {
            return data;
        }


        [[nodiscard]] constexpr auto begin() const noexcept
        {
            return data;
        }

        [[nodiscard]] constexpr auto end() noexcept
        {
            return data + N;
        }

        [[nodiscard]] constexpr auto end() const noexcept
        {
            return data + N;
        }

        constexpr explicit operator std::string_view() const noexcept
        requires (N > 0)
        {
            return std::string_view(data, N);
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

    template<std::size_t N>
    static_string(static_string<N> const &) -> static_string<N>;
}

template<std::size_t N>
std::ostream &operator<<(std::ostream &os, meta::static_string<N> const &str)
{
    return os << static_cast<std::string_view>(str);
}
#endif //MREGEX_UTILITY_STATIC_STRING_HPP