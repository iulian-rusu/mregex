#ifndef CX_STATIC_STRING_H
#define CX_STATIC_STRING_H

#include <cstddef>
#include <array>
#include <type_traits>
#include <iostream>

namespace cx
{
    /**
     * Helper class that implements a constexpr string as a literal type
     *
     * @tparam N    The length of the string (excluding '\0')
     */
    template<std::size_t N>
    struct static_string
    {
        char buffer[N]{};

        constexpr static_string(char const (&str)[N+1]) noexcept
        {
            for (std::size_t i = 0; i < N; ++i)
            {
                buffer[i] = str[i];
            }
        }

        constexpr static_string(static_string const &other) noexcept
        {
            for (std::size_t i = 0; i < N; ++i)
            {
                buffer[i] = other.buffer[i];
            }
        }

        constexpr auto length() const noexcept
        {
            return N;
        }

        constexpr auto begin() noexcept
        {
            return buffer;
        }

        constexpr auto end() noexcept
        {
            return buffer + N;
        }

        constexpr auto substr(std::size_t from, std::size_t count) const noexcept
        {
            return std::string_view(buffer + from, count);
        }

        constexpr explicit operator std::string_view() const noexcept
        {
            return std::string_view(buffer, N);
        }

        constexpr char operator[](std::size_t i) const noexcept
        {
            return buffer[i];
        }
    };

    template<std::size_t N>
    static_string(char const (&)[N]) -> static_string<N-1>;

    template<std::size_t N>
    static_string(static_string<N>) -> static_string<N>;
}

template<std::size_t N>
std::ostream &operator<<(std::ostream &os, cx::static_string<N> const &str)
{
    return os << str.substr(0, N);
}

#endif //CX_STATIC_STRING_H