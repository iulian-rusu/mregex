#ifndef MREGEX_STATIC_STRING_HPP
#define MREGEX_STATIC_STRING_HPP

#include <array>
#include <iosfwd>
#include <type_traits>
#include <string_view>

namespace meta
{
    /**
     * Helper class that implements a constexpr string as a literal type.
     *
     * @tparam N    The length of the string (excluding '\0')
     */
    template<std::size_t N>
    struct static_string
    {
        char data[N]{};

        constexpr static_string(char const (&str)[N + 1]) noexcept
        {
            std::copy(str, str + N, data);
        }

        constexpr static_string(static_string const &other) noexcept
        {
            std::copy(other.data, other.data + N, data);
        }

        [[nodiscard]] constexpr auto length() const noexcept
        {
            return N;
        }

        [[nodiscard]] constexpr auto begin() const noexcept
        {
            return data;
        }

        [[nodiscard]] constexpr auto end() const noexcept
        {
            return data + N;
        }

        constexpr explicit operator std::string_view() const noexcept
        {
            return std::string_view(data, N);
        }

        constexpr char operator[](std::size_t i) const noexcept
        {
            return data[i];
        }
    };

    template<>
    struct static_string<0>
    {
        constexpr static_string(char const (&)[1]) noexcept {}

        [[nodiscard]] static constexpr auto length() noexcept
        {
            return 0;
        }

        [[nodiscard]] static constexpr auto begin() noexcept
        {
            return nullptr;
        }

        [[nodiscard]] static constexpr auto end() noexcept
        {
            return nullptr;
        }

        constexpr explicit operator std::string_view() const noexcept
        {
            return "";
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
#endif //MREGEX_STATIC_STRING_HPP