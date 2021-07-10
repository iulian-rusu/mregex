#ifndef CX_REGEX_RESULT_H
#define CX_REGEX_RESULT_H

#include <tuple>

/**
 * File with data structures that contain matching/searching results
 */
namespace cx
{
    /**
     * Struct that remembers the start coordinate and length of a capture group result
     */
    template<std::size_t>
    struct capture
    {
        std::size_t from;
        std::size_t count;

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

    /**
     * Result returned by all Regex matching/searching functions
     *
     * @tparam N    The number of capture groups (without the implicit 0 group)
     */
    template<std::size_t N>
    struct regex_result
    {
        bool matched{};
        capture_storage<N> captures;
        std::string_view input;

        template<typename Storage, typename = std::enable_if_t<std::is_convertible_v<capture_storage<N>, Storage>>>
        constexpr regex_result(bool m, Storage &&cs, std::string_view sv)
                : captures(std::forward<Storage>(cs)), matched(m), input(sv)
        {}

        constexpr explicit operator bool() const noexcept
        {
            return matched;
        }

        constexpr bool operator==(bool b) const noexcept
        {
            return matched == b;
        }

        [[nodiscard]] constexpr std::size_t end() const noexcept
        {
            return std::get<0>(captures).end();
        }

        template<std::size_t ID>
        constexpr decltype(auto) get() const noexcept
        {
            static_assert(ID <= N, "capture group does not exist");
            return std::get<ID>(captures).evaluate(input);
        }
    };

    template<typename Storage>
    regex_result(bool, Storage &&, std::string_view) -> regex_result<std::tuple_size_v<Storage> - 1>;
}
#endif //CX_REGEX_RESULT_H
