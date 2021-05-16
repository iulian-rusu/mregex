#ifndef CX_REGEX_H
#define CX_REGEX_H

#include "parser.h"

/**
 * Defines a regex type used to search/match input static_string-like objects
 */
namespace cx
{
    template<static_string const pattern>
    struct regex
    {
        static_assert(parser<pattern>::accepted, "syntax error in regular expression");
        using ast = typename parser<pattern>::ast;

        template<string_like Str>
        [[nodiscard]] static constexpr match_result match(Str const &input) noexcept
        {
            auto res = ast::match(input, 0, input.length());
            res.matched = res.count == input.length();
            return res;
        }

        template<string_like Str>
        [[nodiscard]] static constexpr search_result search(Str const &input, std::size_t start_pos = 0) noexcept
        {
            if (start_pos >= input.length())
                return search_result{start_pos, {0, false}};
            if(auto res = ast::match(input, start_pos, input.length()))
                return search_result{start_pos, res};
            return search(input, start_pos + 1);
        }
    };

}
#endif //CX_REGEX_H
