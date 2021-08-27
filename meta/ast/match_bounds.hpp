#ifndef META_MATCH_BOUNDS_HPP
#define META_MATCH_BOUNDS_HPP

#include <concepts>

namespace meta::ast
{
    /**
     * Data structure that determines the matching window bounds for AST nodes.
     */
    template<std::forward_iterator Iter>
    struct match_bounds
    {
        Iter current_iter{};
        std::size_t consume_limit{};

        /**
         * Constructor that takes an integral type and forces a conversion to std::size_t.
         * The sign is not checked because the consume limit is always positive for correct
         * input ranges.
         */
        constexpr match_bounds(Iter it, std::integral auto limit) noexcept
                : current_iter{it}, consume_limit{static_cast<size_t>(limit)}
        {}

        [[nodiscard]] constexpr match_bounds advance(std::size_t count) const noexcept
        {
            auto updated_consume_limit = consume_limit >= count ? consume_limit - count : 0;
            return match_bounds{current_iter + count, updated_consume_limit};
        }
    };

    template<std::forward_iterator Iter>
    match_bounds(Iter, std::integral auto) -> match_bounds<Iter>;
}
#endif //META_MATCH_BOUNDS_HPP