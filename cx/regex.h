#ifndef CX_REGEX_H
#define CX_REGEX_H

#include "parser.h"
#include "generator.h"

/**
 * Defines a Regex type used to match/search input string-like objects
 */
namespace cx
{
    template<static_string const pattern>
    struct regex
    {
        static_assert(parser<pattern>::accepted, "syntax error in regular expression");
        using ast = typename parser<pattern>::ast;
        static constexpr auto capture_count = ast::capture_count;

        template<string_like Str>
        [[nodiscard]] static constexpr auto match(Str const &input) noexcept
        -> regex_result<capture_count>
        {
            capture_storage<capture_count> captures;
            auto res = ast::template match<capture_count>(input, {0, input.length()}, captures);
            res.matched = res.matched && (res.count == input.length());
            std::get<0>(captures) = capture<0>{0, res.count};
            return regex_result{res.matched, std::move(captures), input};
        }

        template<string_like Str>
        [[nodiscard]] static constexpr auto find_first(Str const &input, std::size_t start_pos = 0) noexcept
        -> regex_result<capture_count>
        {
            capture_storage<capture_count> captures;
            while (start_pos < input.length())
            {
                auto res = ast::template match<capture_count>(input, {start_pos, input.length()}, captures);
                if (res)
                {
                    std::get<0>(captures) = capture<0>{start_pos, res.count};
                    return regex_result{true, std::move(captures), input};
                }
                ++start_pos;
            }
            return regex_result{false, std::move(captures), input};
        }

        template<string_like Str>
        [[nodiscard]] static constexpr auto find_all(Str &&input, std::size_t start_pos = 0) noexcept
        {
            return generator
                    {
                            [input = std::forward<Str>(input), pos = start_pos]() mutable {
                                auto result = find_first(input, pos);
                                pos = result.end();
                                return result;
                            }
                    };
        }
    };
}
#endif //CX_REGEX_H
