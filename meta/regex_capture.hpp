#ifndef META_REGEX_CAPTURE_HPP
#define META_REGEX_CAPTURE_HPP

#include <string>
#include <string_view>
#include "utility/concepts.hpp"

namespace meta
{
    /**
     * Base class for a regex capture container.
     *
     * @tparam Str  The type of string like object used to store the captured content
     */
    template<string_like Str>
    class regex_capture_base
    {
    protected:
        Str content;
    public:
        constexpr regex_capture_base()
        noexcept(std::is_nothrow_default_constructible_v<Str>) = default;

        template<typename S>
        constexpr explicit regex_capture_base(S &&s)
        noexcept(std::is_nothrow_move_constructible_v<Str>)
            : content{std::forward<S>(s)}
        {}

        [[nodiscard]] constexpr std::string_view view() const noexcept
        {
            return static_cast<std::string_view>(content);
        }

        [[nodiscard]] constexpr std::size_t length() const noexcept
        {
            return content.length();
        }
    };

    /**
     * Class that holds a view on the captured content of a regex group.
     */
    template<std::size_t>
    class regex_capture_view : public regex_capture_base<std::string_view>
    {
        using regex_capture_base<std::string_view>::content;
    public:
        constexpr regex_capture_view() noexcept = default;

        constexpr explicit regex_capture_view(std::string_view s) noexcept
            : regex_capture_base<std::string_view>{s}
        {}

        constexpr void reset() noexcept
        {
            content = "";
        }
    };

    /**
     * Class that owns a copy of the captured content of a regex group.
     */
    template<std::size_t N>
    class regex_capture : public regex_capture_base<std::string>
    {
    public:
        explicit regex_capture(regex_capture_view<N> const &cap)
        : regex_capture_base<std::string>{cap.view()}
        {}
    };

    /**
     * Defines a std::tuple with N + 1 elements of type regex_capture_view.
     */
    template<typename>
    struct alloc_capture_view_storage;

    template<std::size_t ... Indices>
    struct alloc_capture_view_storage<std::index_sequence<Indices ...>>
    {
        using type = std::tuple<regex_capture_view<Indices> ...>;
    };

    template<std::size_t N>
    using capture_view_storage = typename alloc_capture_view_storage<std::make_index_sequence<N + 1>>::type;

    /**
     * Defines a std::tuple with N + 1 elements of type regex_capture.
     */
    template<typename>
    struct alloc_capture_storage;

    template<std::size_t ... Indices>
    struct alloc_capture_storage<std::index_sequence<Indices ...>>
    {
        using type = std::tuple<regex_capture<Indices> ...>;
    };

    template<std::size_t N>
    using capture_storage = typename alloc_capture_storage<std::make_index_sequence<N + 1>>::type;
}
#endif //META_REGEX_CAPTURE_HPP
