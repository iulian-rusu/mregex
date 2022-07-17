#ifndef MREGEX_DISTANCE_HPP
#define MREGEX_DISTANCE_HPP

#include <iterator>

namespace meta
{
    template<std::ptrdiff_t N, std::forward_iterator Iter>
    constexpr bool distance_less_than(Iter begin, Iter end) noexcept
    {
        std::ptrdiff_t offset = 0;
        while (offset != N && begin != end)
        {
            ++offset;
            ++begin;
        }
        return offset != N;
    }

    template<std::ptrdiff_t N, std::random_access_iterator Iter>
    constexpr bool distance_less_than(Iter begin, Iter end) noexcept
    {
        return std::distance(begin, end) < N;
    }

    template<std::forward_iterator Iter>
    constexpr bool distance_less_than(std::ptrdiff_t required_dist, Iter begin, Iter end) noexcept
    {
        std::ptrdiff_t offset = 0;
        while (offset != required_dist && begin != end)
        {
            ++offset;
            ++begin;
        }
        return offset != required_dist;
    }

    template<std::random_access_iterator Iter>
    constexpr bool distance_less_than(std::ptrdiff_t required_dist, Iter begin, Iter end) noexcept
    {
        return std::distance(begin, end) < required_dist;
    }
}
#endif //MREGEX_DISTANCE_HPP