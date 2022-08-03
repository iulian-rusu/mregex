#ifndef MREGEX_UTILITY_REVERSED_RANGE_VIEW_HPP
#define MREGEX_UTILITY_REVERSED_RANGE_VIEW_HPP

#include <iterator>

namespace meta
{
    namespace ranges = std::ranges;

    /**
     * Minimalist view that reverses the normal iteration direction on a given bidirectional range.
     * This class is used as a workaround since std::views::reverse isn't working in libstdc++.
     *
     * @tparam Range    The bidirectional range type to be reversed
     */
    template<ranges::bidirectional_range Range>
    struct reversed_range_view
    {
        using value_type = ranges::range_value_t<Range>;
        using iterator = std::reverse_iterator<ranges::iterator_t<Range>>;

        constexpr explicit reversed_range_view(Range &range) noexcept : _range{range} {}

        constexpr auto begin() noexcept
        {
            auto it = std::end(_range);
            return std::make_reverse_iterator(it);
        }

        constexpr auto begin() const noexcept
        {
            auto it = std::cend(_range);
            return std::make_reverse_iterator(it);
        }

        constexpr auto end() noexcept
        {
            auto it = std::begin(_range);
            return std::make_reverse_iterator(it);
        }

        constexpr auto end() const noexcept
        {
            auto it = std::cbegin(_range);
            return std::make_reverse_iterator(it);
        }

    private:
        Range &_range;
    };
}
#endif //MREGEX_UTILITY_REVERSED_RANGE_VIEW_HPP