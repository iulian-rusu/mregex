#ifndef CX_REGEX_RESULT_H
#define CX_REGEX_RESULT_H

/**
 * Defines data structures that encapsulate matching/searching results
 */
#include <tuple>

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
    };

    /**
     * Defines recursively a std::tuple with N+1 elements of type capture
     */
    template<std::size_t N>
    struct alloc_capture_storage
    {
        template<typename ...Captures>
        using type = typename alloc_capture_storage<N - 1>::template type<capture<N>, Captures...>;
    };

     template<>
    struct alloc_capture_storage<0>
    {
        template<typename ...Captures>
        using type = std::tuple<capture<0>, Captures ...>;
    };

    template<std::size_t N>
    using capture_storage = typename alloc_capture_storage<N>::template type<>;

    /**
     * Intermediate object returned by AST node matching functions
     */
    struct match_result
    {
        std::size_t count{};
        bool matched{};

        constexpr explicit operator bool() const noexcept
        {
            return matched;
        }

        constexpr bool operator==(bool b) const noexcept
        {
            return matched == b;
        }

        constexpr match_result operator+(match_result const &other) const noexcept
        {
            return match_result{count + other.count, matched || other.matched};
        }

        constexpr match_result &operator+=(match_result const &other) noexcept
        {
            count += other.count;
            matched = matched || other.matched;
            return *this;
        }
    };

    /**
     * Result returned by cx::regex match/search methods
     * @tparam N    The number of capture groups (without the implicit <0> group)
     */
    template<std::size_t N>
    struct capturing_result : capture_storage<N>
    {
        bool matched{};
        std::string_view input;

        constexpr capturing_result(const bool m, capture_storage<N> const& cs, std::string_view sv)
        : capture_storage<N>(cs), matched(m), input(sv) {}

        constexpr explicit operator bool() const noexcept
        {
            return matched;
        }

        constexpr bool operator==(bool b) const noexcept
        {
            return matched == b;
        }

        template<std::size_t ID>
        constexpr decltype(auto) get() noexcept
        {
            static_assert(ID <= N, "capture group does not exist");
            return std::get<ID>(*this).evaluate(input);
        }
    };
}

#endif //CX_REGEX_RESULT_H
