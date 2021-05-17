#ifndef CX_STATIC_STRING_H
#define CX_STATIC_STRING_H

#include <cstddef>
#include <array>
#include <type_traits>

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
        static constexpr auto length = N;

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

        constexpr char operator[](std::size_t i) const noexcept
        {
            return buffer[i];
        }
    };

    template<std::size_t N>
    static_string(char const (&str)[N]) -> static_string<N-1>;

    template<std::size_t N>
    static_string(static_string<N>) -> static_string<N>;
}
#endif //CX_STATIC_STRING_H