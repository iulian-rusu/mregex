#ifndef CX_REGEX_RESULT_H
#define CX_REGEX_RESULT_H

#include "capture.h"

namespace cx
{
    /**
     * Result returned by all Regex matching/searching functions
     *
     * @tparam N    The number of capture groups (without the implicit 0 group)
     */
    template<std::size_t N>
    struct regex_result
    {
        bool matched{};
        capture_storage<N> captures;
        std::string_view input;

        template<typename Storage, typename = std::enable_if_t<std::is_convertible_v<capture_storage<N>, Storage>>>
        constexpr regex_result(bool m, Storage &&cs, std::string_view sv)
                : captures(std::forward<Storage>(cs)), matched(m), input(sv)
        {}

        constexpr explicit operator bool() const noexcept
        {
            return matched;
        }

        constexpr bool operator==(bool b) const noexcept
        {
            return matched == b;
        }

        [[nodiscard]] constexpr std::size_t end() const noexcept
        {
            return std::get<0>(captures).end();
        }

        template<std::size_t ID>
        constexpr decltype(auto) get() const noexcept
        {
            static_assert(ID <= N, "capture group does not exist");
            return std::get<ID>(captures).evaluate(input);
        }
    };

    template<typename Storage>
    regex_result(bool, Storage &&, std::string_view) -> regex_result<std::tuple_size_v<Storage> - 1>;
}

namespace std
{
    // Specialize STL metafunctions for structured binding decomposition

    template <std::size_t N>
    struct tuple_size<cx::regex_result<N>> : std::integral_constant<std::size_t, N + 1> {};

    template <std::size_t ID, std::size_t N>
    struct tuple_element<ID, cx::regex_result<N>>
    {
        using type = std::string_view;
    };
}
#endif //CX_REGEX_RESULT_H