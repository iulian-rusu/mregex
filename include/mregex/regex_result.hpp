#ifndef MREGEX_REGEX_RESULT_HPP
#define MREGEX_REGEX_RESULT_HPP

#include <iosfwd>
#include <tuple>
#include <mregex/utility/tuple_helpers.hpp>
#include <mregex/regex_capture.hpp>

namespace meta
{
    /**
     * Class returned by all regex matching/searching functions.
     *
     * @tparam N        The number of capturing groups
     * @tparam Storage  The storage type used to hold the captures
     */
    template<std::size_t N, capture_storage Storage>
    struct basic_regex_result;

    /**
     * Result that holds views of captured content.
     * If the orginal input string expires before this result object,
     * the behavior is undefined.
     */
    template<std::size_t N, std::forward_iterator Iter>
    using regex_result_view = basic_regex_result<N, regex_capture_view_storage<N, Iter>>;

    /**
     * Result that holds ownership of captured content.
     */
    template<std::size_t N>
    using regex_result = basic_regex_result<N, regex_capture_storage<N>>;

    template<std::size_t N, capture_storage Storage>
    struct basic_regex_result
    {
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
        [[nodiscard]] auto own() const
        requires is_capture_view_v<std::tuple_element_t<0, Storage>>
        {
            auto owning_captures = generate_tuple(captures, [](auto const &capture) {
                return regex_capture{capture};
            });
            return regex_result<N>{matched, std::move(owning_captures)};
        }

        /**
         * Returns the captured group with the specified number.
         * The return type depends on the iterator category used to access
         * the input sequence. If the iterator satisfies std::contiguous_iterator,
         * the method returns a std::string_view of the captured content, otherwise it returns
         * a std::string.
         *
         * @tparam ID   The number of the requested capture group
         * @return      A string-like object containing the captured content
         */
        template<std::size_t ID>
        [[nodiscard]] constexpr decltype(auto) group() const noexcept(is_nothrow_content_v<Storage>)
        {
            static_assert(ID <= N, "capture group does not exist");
            return std::get<ID>(captures).content();
        }

        /**
         * Tuple-like interface for structured binding decomposition.
         * Prefer the group() method for extracting captures.
         */
        template<std::size_t ID>
        constexpr decltype(auto) get() const noexcept(is_nothrow_content_v<Storage>)
        {
            static_assert(ID < N, "tuple element index out of bounds");
            return std::get<ID + 1>(captures).content();
        }

    private:
        bool matched;
        Storage captures;
    };
}

template<std::size_t N, typename Storage>
std::ostream &operator<<(std::ostream &os, meta::basic_regex_result<N, Storage> const &result)
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
#endif //MREGEX_REGEX_RESULT_HPP