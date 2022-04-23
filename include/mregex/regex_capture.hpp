#ifndef MREGEX_REGEX_CAPTURE_HPP
#define MREGEX_REGEX_CAPTURE_HPP

#include <string>
#include <string_view>
#include <mregex/utility/concepts.hpp>

namespace meta
{
    /**
     * Class that holds a view into the captured content of a regex group.
     *
     * @tparam ID   The identifier of the capture group
     * @tparam Iter The forward iterator type used to acces the input sequence
     */
    template<std::size_t ID, std::forward_iterator Iter>
    struct regex_capture_view
    {
        /**
         *  Helper type alias to decide which string type can be constructed from the given
         *  iterator type. Unfortunatelly, std::string_view is only constructible from
         *  contiguous iterator categories.
         */
        using string_type =
                std::conditional_t
                <
                    std::contiguous_iterator<Iter>,
                    std::string_view,
                    std::string
                >;

        constexpr regex_capture_view() = default;

        constexpr explicit regex_capture_view(Iter start, Iter stop) noexcept
                : begin_iter{start}, end_iter{stop}
        {}

        constexpr void clear() noexcept
        {
            end_iter = begin_iter;
        }

        [[nodiscard]] constexpr std::size_t length() const noexcept
        {
            return std::distance(begin_iter, end_iter);
        }

        [[nodiscard]] constexpr auto begin() const noexcept
        {
            return begin_iter;
        }

        [[nodiscard]] constexpr auto end() const noexcept
        {
            return end_iter;
        }

        [[nodiscard]] constexpr auto content() const noexcept
        {
            return string_type{begin_iter, end_iter};
        }

        constexpr explicit operator bool() const noexcept
        {
            return length() > 0;
        }

        constexpr explicit(false) operator string_type() const noexcept
        {
            return content();
        }

    private:
        Iter begin_iter{};
        Iter end_iter{};
    };

    /**
     * Class that owns a copy of the captured content of a regex group.
     *
     * @tparam ID   The identifier of the capture group
     */
    template<std::size_t ID>
    struct regex_capture
    {
        template<std::forward_iterator Iter>
        explicit regex_capture(regex_capture_view<ID, Iter> const &capture_view)
                : captured{capture_view.begin(), capture_view.end()}
        {}

        [[nodiscard]] std::size_t length() const noexcept
        {
            return captured.length();
        }

        [[nodiscard]] auto begin() const noexcept
        {
            return captured.begin();
        }

        [[nodiscard]] auto end() const noexcept
        {
            return captured.end();
        }

        [[nodiscard]] auto &content() & noexcept
        {
            return captured;
        }

        [[nodiscard]] auto const &content() const & noexcept
        {
            return captured;
        }

        [[nodiscard]] auto content() && noexcept
        {
            return std::move(captured);
        }

        constexpr explicit operator bool() const noexcept
        {
            return length() > 0;
        }

        explicit(false) operator std::string() const noexcept
        {
            return content();
        }

    private:
        std::string captured;
    };

    /**
     * Type trait used to check if a given type is an instance of
     * the regex_capture_view template class.
     */
    template<typename T>
    struct is_capture_view : std::false_type {};

    template<std::size_t ID, std::forward_iterator Iter>
    struct is_capture_view<regex_capture_view<ID, Iter>> : std::true_type {};

    template<typename T>
    inline constexpr bool is_capture_view_v = is_capture_view<T>::value;

    /**
     * Defines a std::tuple with N + 1 elements of type regex_capture_view.
     */
    template<typename, typename>
    struct regex_capture_view_allocator;

    template<std::forward_iterator Iter, std::size_t... Indices>
    struct regex_capture_view_allocator<std::index_sequence<Indices ...>, Iter>
    {
        using type = std::tuple<regex_capture_view<Indices, Iter> ...>;
    };

    template<std::size_t N, std::forward_iterator Iter>
    using regex_capture_view_storage = typename regex_capture_view_allocator<std::make_index_sequence<N + 1>, Iter>::type;

    /**
     * Defines a std::tuple with N + 1 elements of type regex_capture.
     */
    template<typename>
    struct regex_capture_allocator;

    template<std::size_t... Indices>
    struct regex_capture_allocator<std::index_sequence<Indices ...>>
    {
        using type = std::tuple<regex_capture<Indices> ...>;
    };

    template<std::size_t N>
    using regex_capture_storage = typename regex_capture_allocator<std::make_index_sequence<N + 1>>::type;

    /**
     * Type trait that checks if the given capture storage type may throw
     * exceptions when accessing contents.
     */
    template<capture_storage Storage>
    inline constexpr bool is_nothrow_content_v = noexcept(std::get<0>(std::declval<Storage>()).content());
}

template<meta::captured_content Capture>
std::ostream &operator<<(std::ostream &os, Capture const &cap)
{
    return os << cap.content();
}
#endif //MREGEX_REGEX_CAPTURE_HPP