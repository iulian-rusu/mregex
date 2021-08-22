#ifndef META_MATCH_BOUNDS_HPP
#define META_MATCH_BOUNDS_HPP

namespace meta::ast
{
    /**
     * Lightweight struct used to transmit matching information to AST nodes.
     * It is separated from meta::match_context for flexibility purposes.
     */
    struct match_bounds
    {
        std::size_t from{};
        std::size_t consume_limit{};

        [[nodiscard]] constexpr match_bounds advance(std::size_t count) const noexcept
        {
            auto updated_consume_limit = consume_limit >= count ? consume_limit - count : 0;
            return match_bounds{from + count, updated_consume_limit};
        }
    };
}
#endif //META_MATCH_BOUNDS_HPP