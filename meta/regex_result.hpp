#ifndef META_REGEX_RESULT_HPP
#define META_REGEX_RESULT_HPP

#include <iosfwd>
#include "capture.hpp"

namespace meta
{
    /**
     * Result returned by all Regex matching/searching functions.
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
            : matched{m}, captures{std::forward<Storage>(cs)}, input{sv}
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
        constexpr decltype(auto) group() const noexcept
        {
            static_assert(ID <= N, "capture group does not exist");
            return std::get<ID>(captures).evaluate(input);
        }

        /**
         * Method used for structured binding decomposition.
         * Prefer the group() method for manually extracting groups.
         */
        template<std::size_t ID>
        constexpr decltype(auto) get() const noexcept
        {
            static_assert(ID < N, "tuple element index out of bounds");
            return std::get<ID + 1>(captures).evaluate(input);
        }
    };

    template<typename Storage>
    regex_result(bool, Storage &&, std::string_view) -> regex_result<std::tuple_size_v<Storage> - 1>;

    template<std::size_t N>
    std::ostream &operator<<(std::ostream &os, regex_result<N> const & result)
    {
        return os << result.template group<0>();
    }
}

namespace std
{
    // Specialize STL metafunctions for structured binding decomposition

    template <std::size_t N>
    struct tuple_size<meta::regex_result<N>> : std::integral_constant<std::size_t, N> {};

    template <std::size_t ID, std::size_t N>
    struct tuple_element<ID, meta::regex_result<N>>
    {
        using type = std::string_view;
    };
}
#endif //META_REGEX_RESULT_HPP