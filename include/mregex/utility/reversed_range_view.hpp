#ifndef MREGEX_UTILITY_REVERSED_RANGE_VIEW_HPP
#define MREGEX_UTILITY_REVERSED_RANGE_VIEW_HPP

#include <iterator>

namespace meta
{
    /**
     * Minimalist view that reverses the normal iteration direction on a given bidirectional range.
     * This adapter is used as a workaround since std::views::reverse isn't working in libstdc++.
     *
     * @tparam Range    The bidirectional range type to be reversed
     */
    template<std::ranges::bidirectional_range Range>
    struct reversed_range_view
    {
        using value_type = std::ranges::range_value_t<Range>;
        using iterator = std::reverse_iterator<std::ranges::iterator_t<Range>>;

        constexpr explicit reversed_range_view(Range &range) noexcept
                : _begin{std::make_reverse_iterator(std::end(range))},
                  _end{std::make_reverse_iterator(std::begin(range))}
        {}
        
        constexpr auto begin() const noexcept
        {
            return _begin;
        }

        constexpr auto end() const noexcept
        {
            return _end;
        }

    private:
        iterator _begin;
        iterator _end;
    };
}
#endif //MREGEX_UTILITY_REVERSED_RANGE_VIEW_HPP