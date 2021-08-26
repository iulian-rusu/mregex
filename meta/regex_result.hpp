#ifndef META_REGEX_RESULT_HPP
#define META_REGEX_RESULT_HPP

#include <iosfwd>
#include <tuple>
#include "regex_capture.hpp"
#include "utility/tuple_helpers.hpp"

namespace meta
{
    /**
     * Class returned by all regex matching/searching functions.
     *
     * @tparam N        The number of capturing groups
     * @tparam Storage  The storage type used to hold the captures
     */
    template<std::size_t N, typename Storage>
    class basic_regex_result;

    /**
     * Result that holds views of captured content.
     * If the orginal input string expires before this result object,
     * the behavior is undefined.
     */
    template<std::size_t N>
    using regex_result_view = basic_regex_result<N, regex_capture_view_storage<N>>;

    /**
     * Result that holds ownership of captured content.
     */
    template<std::size_t N>
    using regex_result = basic_regex_result<N, regex_capture_storage<N>>;

    template<std::size_t N, typename Storage>
    class basic_regex_result
    {
        bool matched{};
        Storage captures;
    public:
        template<typename S>
        constexpr basic_regex_result(bool m, S &&s)
        noexcept(std::is_nothrow_move_constructible_v<Storage>)
        : matched{m}, captures{std::forward<S>(s)}
        {}

        constexpr explicit operator bool() const noexcept
        {
            return matched;
        }

        constexpr bool operator==(bool b) const noexcept
        {
            return matched == b;
        }

        [[nodiscard]] constexpr std::size_t length() const noexcept
        {
            return std::get<0>(captures).length();
        }

        [[nodiscard]] constexpr auto begin() const noexcept
        {
            return  std::get<0>(captures).begin();
        }

        [[nodiscard]] constexpr auto end() const noexcept
        {
            return  std::get<0>(captures).end();
        }

        /**
         * Performs a deep copy of all regex captures and returns
         * a regex result container that owns them.
         *
         * @return  A new regex_result object that holds ownership of captures
         */
        [[nodiscard]] regex_result<N> own() const
        requires std::is_same_v<Storage, regex_capture_view_storage<N>>
        {
            auto owning_captures = tuple_transform(captures, [&](auto const &capture) {
                return regex_capture{capture};
            });
            return {matched, std::move(owning_captures)};
        }

        template<std::size_t ID>
        [[nodiscard]] constexpr decltype(auto) group() const noexcept
        {
            static_assert(ID <= N, "capture group does not exist");
            return std::get<ID>(captures).view();
        }

        /**
         * Method used for structured binding decomposition.
         * Prefer the group() method for extracting groups.
         */
        template<std::size_t ID>
        constexpr decltype(auto) get() const noexcept
        {
            static_assert(ID < N, "tuple element index out of bounds");
            return std::get<ID + 1>(captures).get();
        }
    };
}

template<std::size_t N, typename Storage>
std::ostream &operator<<(std::ostream &os, meta::basic_regex_result<N, Storage> const & result)
{
    return os << result.template group<0>();
}

namespace std
{
    template <std::size_t N, typename Storage>
    struct tuple_size<meta::basic_regex_result<N, Storage>> : std::integral_constant<std::size_t, N> {};

    template <std::size_t ID, std::size_t N, typename Storage>
    struct tuple_element<ID, meta::basic_regex_result<N, Storage>>
    {
        using type = decltype(std::declval<meta::basic_regex_result<N, Storage>>().template get<ID>());
    };
}
#endif //META_REGEX_RESULT_HPP