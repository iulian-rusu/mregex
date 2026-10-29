#ifndef MREGEX_UTILITY_DISTANCE_HPP
#define MREGEX_UTILITY_DISTANCE_HPP

#include <iterator>

namespace meta
{
    template<std::forward_iterator Iter>
    constexpr bool distance_less_than(std::ptrdiff_t limit, Iter begin, Iter end) noexcept
    {
        std::ptrdiff_t offset = 0;
        while (offset != limit && begin != end)
        {
            ++offset;
            ++begin;
        }
        return offset != limit;
    }

    template<std::random_access_iterator Iter>
    constexpr bool distance_less_than(std::ptrdiff_t limit, Iter begin, Iter end) noexcept
    {
        return std::distance(begin, end) < limit;
    }

    template<std::ptrdiff_t N, std::forward_iterator Iter>
    constexpr bool distance_less_than(Iter begin, Iter end) noexcept
    {
        return distance_less_than(N, begin, end);
    }
}
#endif //MREGEX_UTILITY_DISTANCE_HPP