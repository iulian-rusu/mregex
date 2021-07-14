#ifndef CX_CAPTURE_H
#define CX_CAPTURE_H

namespace cx
{
    /**
     * Struct that remembers the start coordinate and length of a capture group result
     */
    template<std::size_t>
    struct capture
    {
        std::size_t from{};
        std::size_t count{};

        constexpr decltype(auto) evaluate(std::string_view sv) const noexcept
        {
            return sv.substr(from, count);
        }

        [[nodiscard]] constexpr std::size_t end() const noexcept
        {
            return from + count;
        }

        constexpr void reset() noexcept
        {
            from = 0u;
            count = 0u;
        }
    };

    /**
     * Defines recursively a std::tuple with N + 1 elements of type capture
     */
    template<std::size_t N>
    struct alloc_capture_storage
    {
        template<typename ... Captures>
        using type = typename alloc_capture_storage<N - 1>::template type<capture<N>, Captures ...>;
    };

    template<>
    struct alloc_capture_storage<0>
    {
        template<typename ... Captures>
        using type = std::tuple<capture<0>, Captures ...>;
    };

    template<std::size_t N>
    using capture_storage = typename alloc_capture_storage<N>::template type<>;
}
#endif //CX_CAPTURE_H
