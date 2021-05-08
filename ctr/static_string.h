#ifndef CTR_STATIC_STRING_H
#define CTR_STATIC_STRING_H

#include <cstddef>
#include <array>
#include <type_traits>

namespace ctr
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

        template<std::size_t M>
        constexpr static_string(static_string<M> const &other) noexcept
        {
            static_assert(N == M, "incompatible static string lengths");
            for (std::size_t i = 0; i < N; ++i)
            {
                buffer[i] = other.buffer[i];
            }
        }

        constexpr char const &operator[](std::size_t i) const
        {
            return buffer[i];
        }
    };

    template<std::size_t N>
    static_string(char const (&str)[N]) -> static_string<N-1>;
}
#endif //CTR_STATIC_STRING_H