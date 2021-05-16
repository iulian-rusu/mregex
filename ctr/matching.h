#ifndef CTR_MATCHING_H
#define CTR_MATCHING_H

#include "parser.h"

/**
 * Defines methods for matching/searching an input string
 */
namespace ctr
{
    template<auto const pattern>
    struct regex
    {
        static_assert(parser<pattern>::accepted, "syntax error in regular expression");
        using ast = typename parser<pattern>::ast;
    };

    template<static_string const pattern, typename StringLike>
    [[nodiscard]] constexpr match_result match(StringLike const &input) noexcept
    {
        using ast = typename regex<pattern>::ast;

        auto res = ast::match(input, 0, input.length());
        res.matched = res.count == input.length();
        return res;
    }

    template<static_string const pattern, typename StringLike>
    [[nodiscard]] constexpr search_result search(StringLike const &input, std::size_t start_pos = 0) noexcept
    {
        if (start_pos >= input.length())
            return search_result{start_pos, {0, false}};
        using ast = typename regex<pattern>::ast;

        if(auto res = ast::match(input, start_pos, input.length()))
            return search_result{start_pos, res};
        return search<static_string<pattern.length>(pattern)>(input, start_pos + 1);
    }
}
#endif //CTR_MATCHING_H
