#ifndef MREGEX_REVERSED_RANGE_VIEW_HPP
#define MREGEX_REVERSED_RANGE_VIEW_HPP

#include <iterator>

namespace meta
{
    namespace ranges = std::ranges;

    /**
     * View that reverses the normal iteration direction on a given bidirectional range.
     * This class is used as a workaround for std::views::reverse not working in libstdc++.
     *
     * @tparam Range    The type of the bidirectional range to be reversed
     */
    template<ranges::bidirectional_range Range>
    struct reversed_range_view
    {
        using value_type = ranges::range_value_t<Range>;
        using iterator = std::reverse_iterator<ranges::iterator_t<Range>>;

        constexpr explicit reversed_range_view(Range &r) noexcept : range{r} {}

        constexpr auto begin() const noexcept
        {
            auto it = std::end(range);
            return std::make_reverse_iterator(it);
        }

        constexpr auto end() const noexcept
        {
            auto it = std::begin(range);
            return std::make_reverse_iterator(it);
        }

        constexpr auto cbegin() const noexcept
        {
            auto it = std::cend(range);
            return std::make_reverse_iterator(it);
        }

        constexpr auto cend() const noexcept
        {
            auto it = std::cbegin(range);
            return std::make_reverse_iterator(it);
        }

    private:
        Range &range;
    };
}
#endif //MREGEX_REVERSED_RANGE_VIEW_HPP