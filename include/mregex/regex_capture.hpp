#ifndef MREGEX_REGEX_CAPTURE_HPP
#define MREGEX_REGEX_CAPTURE_HPP

#include <iterator>
#include <string>
#include <string_view>
#include <mregex/ast/traits.hpp>
#include <mregex/utility/concepts.hpp>

namespace meta
{
    /**
     * Class that holds a view into the captured content of a regex group.
     *
     * @tparam Iter The forward iterator type used to acces the input sequence
     * @tparam Name The name of the capturing group (optional)
     */
    template<std::forward_iterator Iter, typename Name = symbol::unnamed>
    struct regex_capture_view
    {
        constexpr regex_capture_view() noexcept = default;

        constexpr explicit regex_capture_view(Iter begin, Iter end) noexcept
            : _begin{begin}, _end{end}
        {}

        constexpr void clear() noexcept
        {
            _end = _begin;
        }

        [[nodiscard]] constexpr std::size_t length() const noexcept
        {
            return std::distance(_begin, _end);
        }

        [[nodiscard]] constexpr auto begin() const noexcept
        {
            return _begin;
        }

        [[nodiscard]] constexpr auto end() const noexcept
        {
            return _end;
        }

        [[nodiscard]] constexpr auto content() const noexcept(std::contiguous_iterator<Iter>)
        {
            if constexpr (std::contiguous_iterator<Iter>)
                return std::string_view{_begin, _end};
            else
                return std::string{_begin, _end};
        }

        [[nodiscard]] constexpr auto &operator[](std::size_t index)
        requires std::random_access_iterator<Iter>
        {
            return _begin[index];
        }

        [[nodiscard]] constexpr auto const &operator[](std::size_t index) const
        requires std::random_access_iterator<Iter>
        {
            return _begin[index];
        }

        constexpr explicit operator bool() const noexcept
        {
            return length() > 0;
        }

        constexpr explicit(false) operator std::string_view() const noexcept
        requires std::contiguous_iterator<Iter>
        {
            return {_begin, _end};
        }

    private:
        Iter _begin{};
        Iter _end{};
    };

    /**
     * Class that holds ownership on the captured content of a regex group.
     *
     * @tparam Name The name of the capturing group (optional)
     */
    template<typename Name = symbol::unnamed>
    struct regex_capture
    {
        regex_capture() noexcept = default;

        template<std::forward_iterator Iter>
        explicit regex_capture(regex_capture_view<Iter, Name> const &capture_view)
            : _capture{capture_view.begin(), capture_view.end()}
        {}

        [[nodiscard]] std::size_t length() const noexcept
        {
            return _capture.length();
        }

        [[nodiscard]] auto begin() noexcept
        {
            return _capture.begin();
        }

        [[nodiscard]] auto begin() const noexcept
        {
            return _capture.cbegin();
        }

        [[nodiscard]] auto end() noexcept
        {
            return _capture.end();
        }

        [[nodiscard]] auto end() const noexcept
        {
            return _capture.cend();
        }

        [[nodiscard]] auto &content() & noexcept
        {
            return _capture;
        }

        [[nodiscard]] auto const &content() const & noexcept
        {
            return _capture;
        }

        [[nodiscard]] auto &&content() && noexcept
        {
            return _capture;
        }

        [[nodiscard]] auto const &&content() const && noexcept
        {
            return _capture;
        }

        [[nodiscard]] auto &operator[](std::size_t index)
        {
            return _capture[index];
        }

        [[nodiscard]] auto const &operator[](std::size_t index) const
        {
            return _capture[index];
        }

        explicit operator bool() const noexcept
        {
            return length() > 0;
        }

        explicit(false) operator std::string_view() const noexcept
        {
            return _capture;
        }

    private:
        std::string _capture;
    };

    /**
     * Type trait used to check if a given capture type is an instance of
     * the regex_capture_view template.
     */
    template<captured_content>
    inline constexpr bool is_capture_view = false;

    template<std::forward_iterator Iter, typename Name>
    inline constexpr bool is_capture_view<regex_capture_view<Iter, Name>> = true;

    /**
     * Defines a std::tuple used to store views into regex captures.
     */
    template<typename, typename>
    struct regex_capture_view_allocator;

    template<std::forward_iterator Iter, typename... Names>
    struct regex_capture_view_allocator<type_sequence<Names ...>, Iter>
    {
        using type = std::tuple<regex_capture_view<Iter>, regex_capture_view<Iter, Names> ...>;
    };

    template<typename NameSpec, std::forward_iterator Iter>
    using regex_capture_view_storage = typename regex_capture_view_allocator<NameSpec, Iter>::type;

    /**
     * Defines a std::tuple used to store memory-owning regex captures.
     */
    template<typename>
    struct regex_capture_allocator;

    template<typename... Names>
    struct regex_capture_allocator<type_sequence<Names ...>>
    {
        using type = std::tuple<regex_capture<>, regex_capture<Names> ...>;
    };

    template<typename NameSpec>
    using regex_capture_storage = typename regex_capture_allocator<NameSpec>::type;

    /**
     * Metafunction used to rename a given regex capture type using pattern matching.
     *
     * @tparam Capture  The capture type to be renamed
     * @tparam Name     The new name for the capture type
     */
    template<captured_content Capture, typename Name>
    struct rename_capture;

    template<captured_content Capture, typename Name>
    using rename_capture_t = typename rename_capture<Capture, Name>::type;

    template<std::forward_iterator Iter, typename OldName, typename Name>
    struct rename_capture<regex_capture_view<Iter, OldName>, Name>
    {
        using type = regex_capture_view<Iter, Name>;
    };

    template<typename OldName, typename Name>
    struct rename_capture<regex_capture<OldName>, Name>
    {
        using type = regex_capture<Name>;
    };
}

template<meta::captured_content Capture>
std::ostream &operator<<(std::ostream &os, Capture const &capture)
{
    std::copy(capture.begin(), capture.end(), std::ostream_iterator<char>{os});
    return os;
}
#endif //MREGEX_REGEX_CAPTURE_HPP