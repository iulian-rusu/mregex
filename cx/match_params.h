#ifndef CX_MATCH_PARAMS_H
#define CX_MATCH_PARAMS_H

namespace cx
{
    /**
     * Lightweight struct used to transmit matching information to AST nodes
     * It is separated from cx::match_context to minimize mutability
     */
    struct match_params
    {
        std::size_t from{};
        std::size_t consume_limit{};

        [[nodiscard]] constexpr match_params consume(std::size_t count) const noexcept
        {
            auto updated_consume_limit = consume_limit >= count ? consume_limit - count : 0;
            return match_params{from + count, updated_consume_limit};
        }
    };
}
#endif //CX_MATCH_PARAMS_H