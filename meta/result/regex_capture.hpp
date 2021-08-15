#ifndef META_REGEX_CAPTURE_HPP
#define META_REGEX_CAPTURE_HPP

#include <string_view>
#include <string>

namespace meta
{
    /**
     * Struct that remembers the start coordinate and span of a captured result.
     */
    template<std::size_t>
    class regex_capture
    {
        std::size_t from{};
        std::size_t consumed{};
    public:
        constexpr regex_capture() noexcept = default;

        constexpr regex_capture(std::size_t f, std::size_t c) noexcept
            : from{f}, consumed{c}
        {}

        [[nodiscard]] constexpr std::string_view evaluate(std::string_view input) const noexcept
        {
            auto const begin = input.cbegin() + from;
            return {begin, begin + consumed};
        }

        [[nodiscard]] constexpr std::size_t end() const noexcept
        {
            return from + consumed;
        }

        constexpr void reset() noexcept
        {
            from = 0u;
            consumed = 0u;
        }
    };

    template<std::size_t N>
    class owning_regex_capture
    {
        std::string content{};
    public:
        owning_regex_capture() noexcept = default;

        owning_regex_capture(regex_capture<N> const &cap, std::string_view str)
            : content{cap.evaluate(str)}
        {}

        [[nodiscard]] std::string_view get() const noexcept
        {
            return content;
        }
    };

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

    /**
     * Defines a std::tuple with N + 1 elements of type owning_regex_capture.
     */
    template<typename>
    struct alloc_owning_capture_storage;

    template<std::size_t ... Indices>
    struct alloc_owning_capture_storage<std::index_sequence<Indices ...>>
    {
        using type = std::tuple<owning_regex_capture<Indices> ...>;
    };

    template<std::size_t N>
    using owning_capture_storage = typename alloc_owning_capture_storage<std::make_index_sequence<N + 1>>::type;
}
#endif //META_REGEX_CAPTURE_HPP