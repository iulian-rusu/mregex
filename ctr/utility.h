#ifndef CTR_UTILITY_H
#define CTR_UTILITY_H
#include <type_traits>

namespace ctr
{
    // some utility metafunctions

    template<typename First, typename ...>
    struct first
    {
        using type = First;
    };

    template<typename ... Elems>
    using first_t = typename first<Elems ...>::type;

    template<typename First, typename ... Rest>
    struct last
    {
        using type = typename last<Rest ...>::type;
    };

    template<typename First>
    struct last<First>
    {
        using type = First;
    };

    template<typename ... Elems>
    using last_t = typename last<Elems ...>::type;

    template<typename First, typename Second>
    struct pair
    {
        using first = First;
        using second = Second;
    };

    template<typename First, typename ... Rest>
    struct count_captures
    {
        static constexpr std::size_t capture_count = First::capture_count + count_captures<Rest ...>::capture_count;
    };

    template<typename First>
    struct count_captures<First>
    {
        static constexpr std::size_t capture_count = First::capture_count;
    };

    struct match_result
    {
        int count;
        bool matched;

        constexpr explicit operator bool() const noexcept { return matched; }

        constexpr match_result operator+(match_result const &other) const noexcept
        {
            return {count + other.count, matched || other.matched};
        }

        constexpr match_result &operator+=(match_result const &other) noexcept
        {
            count += other.count;
            matched = matched || other.matched;
            return *this;
        }
    };
}

#endif //CTR_UTILITY_H
