#ifndef MREGEX_REGEX_CAPTURE_HPP
#define MREGEX_REGEX_CAPTURE_HPP

#include <string>
#include <iterator>
#include <string_view>
#include <mregex/ast/capture_name_specifier.hpp>
#include <mregex/utility/concepts.hpp>

namespace meta
{
    /**
     * Class that holds a view into the captured content of a regex group.
     *
     * @tparam ID   The identifier of the capturing group
     * @tparam Iter The forward iterator type used to acces the input sequence
     * @tparam Name The name of the capturing group (optional)
     */
    template<std::forward_iterator Iter, typename Name = symbol::unnamed>
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

        explicit(false) operator std::string_view() const noexcept
        requires std::contiguous_iterator<Iter>
        {
            return {begin_iter, end_iter};
        }

        explicit(false) operator std::string() const noexcept
        {
            return {begin_iter, end_iter};
        }

    private:
        Iter begin_iter{};
        Iter end_iter{};
    };

    /**
     * Class that owns a copy of the captured content of a regex group.
     *
     * @tparam ID   The identifier of the capturing group
     * @tparam Name The name of the capturing group (optional)
     */
    template<typename Name = symbol::unnamed>
    struct regex_capture
    {
        template<std::forward_iterator Iter>
        explicit regex_capture(regex_capture_view<Iter, Name> const &capture_view)
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

        [[nodiscard]] auto &&content() && noexcept
        {
            return captured;
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

    template<std::forward_iterator Iter, typename Name>
    struct is_capture_view<regex_capture_view<Iter, Name>> : std::true_type {};

    template<typename T>
    inline constexpr bool is_capture_view_v = is_capture_view<T>::value;

    /**
     * Defines a std::tuple used to store views into regex captures.
     */
    template<typename, typename>
    struct regex_capture_view_allocator;

    template<std::forward_iterator Iter, typename... Names>
    struct regex_capture_view_allocator<stack<Names ...>, Iter>
    {
        using type = std::tuple<regex_capture_view<Iter>, regex_capture_view<Iter, Names> ...>;
    };

    template<typename AST, std::forward_iterator Iter>
    using regex_capture_view_storage = typename regex_capture_view_allocator<ast::capture_name_spec_t<AST>, Iter>::type;

    /**
     * Defines a std::tuple used to store memory-owning regex captures.
     */
    template<typename>
    struct regex_capture_allocator;

    template<typename... Names>
    struct regex_capture_allocator<stack<Names ...>>
    {
        using type = std::tuple<regex_capture<>, regex_capture<Names> ...>;
    };

    template<typename AST>
    using regex_capture_storage = typename regex_capture_allocator<ast::capture_name_spec_t<AST>>::type;

    /**
     * Type trait that checks if the given capture storage type may throw
     * exceptions when accessing contents.
     */
    template<capture_storage Storage>
    inline constexpr bool is_nothrow_content_v = noexcept(std::get<0>(std::declval<Storage>()).content());

    /**
     * Metafunction used to rename a given regex capture type using pattern matching.
     *
     * @tparam Capture  The capture type to be renamed
     * @tparam Name     The new name for the capture type
     */
    template< typename Capture, typename Name>
    struct rename_capture;

    template<typename Capture, typename Name>
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

    /**
     * Metafunction that generates a valid named capture type for a given capture storage type.
     *
     * @tparam Storage  The capture storage used to deduce the generated type
     * @tparam Name     The name of the generated capture type
     */
    template<capture_storage Storage, typename Name>
    using named_capture_type_for = rename_capture_t<std::tuple_element_t<0, Storage>, Name>;
}

template<meta::captured_content Capture>
std::ostream &operator<<(std::ostream &os, Capture const &cap)
{
    std::copy(cap.begin(), cap.end(), std::ostream_iterator<char>{os});
    return os;
}
#endif //MREGEX_REGEX_CAPTURE_HPP