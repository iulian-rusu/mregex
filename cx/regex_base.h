#ifndef CX_REGEX_BASE_H
#define CX_REGEX_BASE_H

#include "generator.h"
#include "regex_result.h"
#include "atomic.h"
#include "match_context.h"

namespace cx
{
    /**
     * Base for all Regex-like types.
     *
     * @tparam AST      The Abstract Syntax Tree of the Regex object
     * @tparam Flags    Optional flags for matching
     */
    template<typename AST, typename ... Flags>
    struct regex_base
    {
        using ast = AST;
        static constexpr std::size_t capture_count = ast::capture_count;

        template<typename ... ExtraFlags>
        struct with_flags
        {
            using match_context = create_match_context<regex_base, Flags ..., ExtraFlags ...>;

            template<string_like Str>
            [[nodiscard]] static constexpr auto match(Str const &input) noexcept
            -> regex_result<capture_count>
            {
                match_context ctx{};
                auto res = ast::match(input, {0, input.length()}, ctx);
                res.matched = res.matched && (res.consumed == input.length());
                if (!res.matched)
                    ctx.clear_captures();
                else
                    std::get<0>(ctx.captures) = capture<0>{0, res.consumed};
                return regex_result{res.matched, std::move(ctx.captures), input};
            }

            template<string_like Str>
            [[nodiscard]] static constexpr auto find_first(Str const &input, std::size_t start_pos = 0) noexcept
            -> regex_result<capture_count>
            {
                match_context ctx{};
                std::size_t str_length = input.length();
                while (start_pos < str_length)
                {
                    auto res = ast::match(input, {start_pos, str_length}, ctx);
                    if (res)
                    {
                        std::get<0>(ctx.captures) = capture<0>{start_pos, res.consumed};
                        return regex_result{true, std::move(ctx.captures), input};
                    }

                    if constexpr (has_atomic_group_v<ast>)
                        ctx.clear_captures();
                    ++start_pos;
                }
                ctx.clear_captures();
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

        template<string_like Str>
        [[nodiscard]] static constexpr decltype(auto) match(Str const &input) noexcept
        {
            return with_flags<>::match(input);
        }

        template<string_like Str>
        [[nodiscard]] static constexpr decltype(auto) find_first(Str const &input, std::size_t start_pos = 0) noexcept
        {
            return with_flags<>::find_first(input, start_pos);
        }

        template<string_like Str>
        [[nodiscard]] static constexpr decltype(auto) find_all(Str &&input, std::size_t start_pos = 0) noexcept
        {
            return with_flags<>::find_all(input, start_pos);
        }
    };
}
#endif //CX_REGEX_BASE_H