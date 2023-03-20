#ifndef MREGEX_UTILITY_REVERSED_RANGE_VIEW_HPP
#define MREGEX_UTILITY_REVERSED_RANGE_VIEW_HPP

#include <iterator>

namespace meta
{
    /**
     * Minimalist view that reverses the normal iteration direction on a given bidirectional range.
     * This class is used as a workaround since std::views::reverse isn't working in libstdc++.
     *
     * @tparam Range    The bidirectional range type to be reversed
     */
    template<std::ranges::bidirectional_range Range>
    struct reversed_range_view
    {
        using value_type = std::ranges::range_value_t<Range>;
        using iterator = std::reverse_iterator<std::ranges::iterator_t<Range>>;

        constexpr explicit reversed_range_view(Range &range) noexcept : _range{range} {}

        constexpr auto begin() noexcept
        {
            return std::make_reverse_iterator(std::end(_range));
        }

        constexpr auto begin() const noexcept
        {
            return std::make_reverse_iterator(std::cend(_range));
        }

        constexpr auto end() noexcept
        {
            return std::make_reverse_iterator(std::begin(_range));
        }

        constexpr auto end() const noexcept
        {
            return std::make_reverse_iterator(std::cbegin(_range));
        }

    private:
        Range &_range;
    };
}
#endif //MREGEX_UTILITY_REVERSED_RANGE_VIEW_HPP