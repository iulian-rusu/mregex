#ifndef META_REGEX_CAPTURE_HPP
#define META_REGEX_CAPTURE_HPP

#include <string>
#include <string_view>
#include "utility/concepts.hpp"

namespace meta
{
    /**
     * Class that holds a view into the captured content of a regex group.
     *
     * @tparam Iter The forward iterator type used to acces the input sequence
     */
    template<std::size_t, std::forward_iterator Iter>
    class regex_capture_view
    {
        Iter begin_iter;
        Iter end_iter;

    public:
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
        requires std::contiguous_iterator<Iter>
        {
            // Views can only be constructed from contiguous memory
            return std::string_view{begin_iter, end_iter};
        }

        [[nodiscard]] auto content() const
        requires (!std::contiguous_iterator<Iter>)
        {
            return std::string{begin_iter, end_iter};
        }
    };

    /**
     * Class that owns a copy of the captured content of a regex group.
     */
    template<std::size_t N>
    class regex_capture
    {
        std::string captured;

    public:
        template<std::forward_iterator Iter>
        explicit regex_capture(regex_capture_view<N, Iter> const &capture_view)
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
    };

    /**
     * Type trait used to check if a given type is an instance of
     * the regex_capture_view template class.
     */
    template<typename T>
    struct is_capture_view : std::false_type {};

    template<std::size_t N, std::forward_iterator Iter>
    struct is_capture_view<regex_capture_view<N, Iter>> : std::true_type {};

    template<typename T>
    inline constexpr bool is_capture_view_v = is_capture_view<T>::value;

    /**
     * Defines a std::tuple with N + 1 elements of type regex_capture_view.
     */
    template<typename, typename>
    struct alloc_capture_view_storage;

    template<std::forward_iterator Iter, std::size_t... Indices>
    struct alloc_capture_view_storage<std::index_sequence<Indices ...>, Iter>
    {
        using type = std::tuple<regex_capture_view<Indices, Iter> ...>;
    };

    template<std::size_t N, std::forward_iterator Iter>
    using regex_capture_view_storage = typename alloc_capture_view_storage<std::make_index_sequence<N + 1>, Iter>::type;

    /**
     * Defines a std::tuple with N + 1 elements of type regex_capture.
     */
    template<typename>
    struct alloc_capture_storage;

    template<std::size_t... Indices>
    struct alloc_capture_storage<std::index_sequence<Indices ...>>
    {
        using type = std::tuple<regex_capture<Indices> ...>;
    };

    template<std::size_t N>
    using regex_capture_storage = typename alloc_capture_storage<std::make_index_sequence<N + 1>>::type;

    /**
     * Type trait that checks if the given capture storage type may throw
     * exceptions when accessing contents.
     */
    template<capture_storage Storage>
    inline constexpr bool is_nothrow_content_v = noexcept(std::get<0>(std::declval<Storage>()).content());
}
#endif //META_REGEX_CAPTURE_HPP
