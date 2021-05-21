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
        static constexpr auto capture_count = ast::capture_count;

        template<string_like SL>
        [[nodiscard]] static constexpr auto match(SL const &input) noexcept
        -> capturing_result<capture_count>
        {
            capture_storage<capture_count> captures;
            auto res = ast::template match<capture_count>(input, 0, input.length(), captures);
            res.matched = res.matched && (res.count == input.length());
            std::get<0>(captures) = capture<0>{0, res.count};
            return capturing_result{res.matched, std::move(captures), input};
        }

        template<string_like SL>
        [[nodiscard]] static constexpr auto search(SL const &input, std::size_t start_pos = 0) noexcept
        -> capturing_result<capture_count>
        {
            capture_storage<capture_count> captures;
            while (start_pos < input.length())
            {
                auto res = ast::template match<capture_count>(input, start_pos, input.length(), captures);
                if (res)
                {
                    std::get<0>(captures) = capture<0>{start_pos, res.count};
                    return capturing_result{res.matched, std::move(captures), input};
                }
                ++start_pos;
            }
            return capturing_result{false, std::move(captures), input};
        }
    };

}
#endif //CX_REGEX_H
