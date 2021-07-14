#ifndef CX_REGEX_H
#define CX_REGEX_H

#include "parser.h"
#include "generator.h"
#include "ast/atomic.h"
#include "match_context.h"

namespace cx
{
    template<static_string const pattern, typename ... Flags>
    struct regex
    {
        static_assert(parser<pattern>::accepted, "syntax error in regular expression");
        using ast = typename parser<pattern>::ast;
        static constexpr auto capture_count = ast::capture_count;

        struct flags
        {
            static constexpr bool ignore_case = is_any_of_v<flag::ignore_case, Flags ...>;
            static constexpr bool lazy_cycle = is_any_of_v<flag::lazy_cycle, Flags ...>;
        };

        template<string_like Str>
        [[nodiscard]] static constexpr auto match(Str const &input) noexcept
        -> regex_result<capture_count>
        {
            match_context<regex> ctx{};
            auto res = ast::template match<regex>(input, {0, input.length()}, ctx);
            res.matched = res.matched && (res.count == input.length());
            std::get<0>(ctx.captures) = capture<0>{0, res.count};
            return regex_result{res.matched, std::move(ctx.captures), input};
        }

        template<string_like Str>
        [[nodiscard]] static constexpr auto find_first(Str const &input, std::size_t start_pos = 0) noexcept
        -> regex_result<capture_count>
        {
            match_context<regex> ctx{};
            while (start_pos < input.length())
            {
                auto res = ast::template match<regex>(input, {start_pos, input.length()}, ctx);
                if (res)
                {
                    std::get<0>(ctx.captures) = capture<0>{start_pos, res.count};
                    return regex_result{true, std::move(ctx.captures), input};
                }
                if constexpr (has_atomic_group_v<ast>)
                {
                    ctx.clear_captures();
                }
                ++start_pos;
            }
            return regex_result{false, std::move(ctx.captures), input};
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
