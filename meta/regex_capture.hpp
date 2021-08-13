#ifndef META_REGEX_CAPTURE_HPP
#define META_REGEX_CAPTURE_HPP

namespace meta
{
    /**
     * Struct that remembers the start coordinate and span of a captured result.
     */
    template<std::size_t>
    struct regex_capture
    {
        std::size_t from{};
        std::size_t consumed{};

        [[nodiscard]] constexpr std::string_view evaluate(std::string_view input) const noexcept
        {
            return input.substr(from, consumed);
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

    /**
     * Defines recursively a std::tuple with N + 1 elements of type regex_capture.
     */
    template<std::size_t N>
    struct alloc_capture_storage
    {
        template<typename ... Captures>
        using type = typename alloc_capture_storage<N - 1>::template type<regex_capture<N>, Captures ...>;
    };

    template<>
    struct alloc_capture_storage<0>
    {
        template<typename ... Captures>
        using type = std::tuple<regex_capture<0>, Captures ...>;
    };

    template<std::size_t N>
    using capture_storage = typename alloc_capture_storage<N>::template type<>;
}
#endif //META_REGEX_CAPTURE_HPP