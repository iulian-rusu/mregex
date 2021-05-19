#ifndef CX_REGEX_H
#define CX_REGEX_H

#include "parser.h"

/**
 * Defines a regex type used to search/match input string-like objects
 */
namespace cx
{
    template<static_string const pattern>
    struct regex
    {
        static_assert(parser<pattern>::accepted, "syntax error in regular expression");
        using ast = typename parser<pattern>::ast;

        template<string_like SL>
        [[nodiscard]] static constexpr auto match(SL const &input) noexcept
        -> match_result
        {
            auto res = ast::match(input, 0, input.length());
            res.matched = res.matched && (res.count == input.length());
            return res;
        }

        template<string_like SL>
        [[nodiscard]] static constexpr auto search(SL const &input, std::size_t start_pos = 0) noexcept
        -> search_result
        {
            if (start_pos >= input.length())
                return search_result{start_pos, {0, false}};
            if (auto res = ast::match(input, start_pos, input.length()))
                return search_result{start_pos, res};
            return search(input, start_pos + 1);
        }
    };

}
#endif //CX_REGEX_H
