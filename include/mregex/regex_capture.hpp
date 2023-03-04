#ifndef MREGEX_REGEX_CAPTURE_HPP
#define MREGEX_REGEX_CAPTURE_HPP

#include <iterator>
#include <string>
#include <string_view>
#include <mregex/ast/traits.hpp>
#include <mregex/symbols/names.hpp>
#include <mregex/utility/concepts.hpp>

namespace meta
{
    template<typename Name>
    struct regex_capture_base
    {
        static constexpr bool has_name() noexcept
        {
            return !std::is_same_v<Name, symbol::unnamed>;
        }

        static constexpr std::string_view name() noexcept
        {
            if constexpr (has_name())
                return Name::value;
            return "";
        }
    };

    /**
     * Class that holds a view into the captured content of a regex group.
     *
     * @tparam Iter The forward iterator type used to acces the input sequence
     * @tparam Name The name of the capturing group (optional)
     */
    template<std::forward_iterator Iter, typename Name = symbol::unnamed>
    struct regex_capture_view : regex_capture_base<Name>
    {
        static constexpr bool is_contiguous_view = std::contiguous_iterator<Iter>;

        constexpr regex_capture_view() noexcept = default;

        constexpr explicit regex_capture_view(Iter begin, Iter end) noexcept
            : _begin{begin}, _end{end}
        {}

        constexpr void clear() noexcept
        {
            _end = _begin;
        }

        constexpr std::size_t length() const noexcept
        {
            return std::distance(_begin, _end);
        }

        constexpr auto begin() const noexcept
        {
            return _begin;
        }

        constexpr auto end() const noexcept
        {
            return _end;
        }

        [[nodiscard]] constexpr auto content() const noexcept(is_contiguous_view)
        {
            if constexpr (is_contiguous_view)
                return std::string_view{_begin, _end};
            else
                return std::string{_begin, _end};
        }

        constexpr auto &operator[](std::size_t index) noexcept
        requires std::random_access_iterator<Iter>
        {
            return _begin[index];
        }

        constexpr auto const &operator[](std::size_t index) const noexcept
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
    struct regex_capture : regex_capture_base<Name>
    {
        regex_capture() noexcept = default;

        template<std::forward_iterator Iter>
        explicit regex_capture(regex_capture_view<Iter, Name> const &capture_view)
            : _capture{capture_view.begin(), capture_view.end()}
        {}

        std::size_t length() const noexcept
        {
            return _capture.length();
        }

        auto begin() noexcept
        {
            return _capture.begin();
        }

        auto begin() const noexcept
        {
            return _capture.cbegin();
        }

        auto end() noexcept
        {
            return _capture.end();
        }

        auto end() const noexcept
        {
            return _capture.cend();
        }

        auto &content() & noexcept
        {
            return _capture;
        }

        auto const &content() const & noexcept
        {
            return _capture;
        }

        auto &&content() && noexcept
        {
            return _capture;
        }

        auto const &&content() const && noexcept
        {
            return _capture;
        }

        auto &operator[](std::size_t index) noexcept
        {
            return _capture[index];
        }

        auto const &operator[](std::size_t index) const noexcept
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
    template<captured_content Capture>
    inline constexpr bool is_capture_view = false;

    template<std::forward_iterator Iter, typename Name>
    inline constexpr bool is_capture_view<regex_capture_view<Iter, Name>> = true;

    /**
     * Defines a std::tuple used to store views into regex captures.
     */
    template<std::forward_iterator Iter, typename NameSpec>
    struct regex_capture_view_allocator;

    template<std::forward_iterator Iter, typename... Names>
    struct regex_capture_view_allocator<Iter, type_sequence<Names ...>>
    {
        using storage_type = std::tuple<regex_capture_view<Iter>, regex_capture_view<Iter, Names> ...>;
    };

    template<std::forward_iterator Iter, typename NameSpec>
    using regex_capture_view_storage = typename regex_capture_view_allocator<Iter, NameSpec>::storage_type;

    /**
     * Defines a std::tuple used to store memory-owning regex captures.
     */
    template<typename NameSpec>
    struct regex_capture_allocator;

    template<typename... Names>
    struct regex_capture_allocator<type_sequence<Names ...>>
    {
        using storage_type = std::tuple<regex_capture<>, regex_capture<Names> ...>;
    };

    template<typename NameSpec>
    using regex_capture_storage = typename regex_capture_allocator<NameSpec>::storage_type;

    /**
     * Metafunction used to rename a given regex capture type using pattern matching.
     *
     * @tparam Capture  The capture type to be renamed
     * @tparam Name     The new name for the capture type
     */
    template<captured_content Capture, typename Name>
    struct rename_capture;

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

    template<captured_content Capture, typename Name>
    using rename_capture_t = typename rename_capture<Capture, Name>::type;
}

template<meta::captured_content Capture>
std::ostream &operator<<(std::ostream &os, Capture const &capture)
{
    std::copy(capture.begin(), capture.end(), std::ostream_iterator<char>{os});
    return os;
}
#endif //MREGEX_REGEX_CAPTURE_HPP