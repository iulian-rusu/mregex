#ifndef META_REGEX_BASE_HPP
#define META_REGEX_BASE_HPP

#include "generator.hpp"
#include "regex_result.hpp"
#include "match_context.hpp"
#include "ast/ast_traits.hpp"
#include "ast/ast.hpp"

namespace meta
{
    /**
     * Base for all regex-like types.
     *
     * @tparam AST      The Abstract Syntax Tree of the regex object
     * @tparam Flags    Optional flags for matching
     */
    template<typename AST, typename ... Flags>
    struct regex_base
    {
        using ast_type = AST;

        static constexpr std::size_t capture_count = ast_type::capture_count;
        static constexpr std::size_t atomic_count = ast_type::atomic_count;

        constexpr regex_base() noexcept = default;

        template<typename ... ExtraFlags>
        struct with_flags
        {
            using match_context = create_match_context<regex_base, Flags ..., ExtraFlags ...>;

            template<string_like Str>
            [[nodiscard]] static constexpr auto match(Str const &input) noexcept
            -> regex_result<capture_count>
            {
                match_context ctx{};
                auto res = ast_type::match(input, {0, input.length()}, ctx);
                res.matched = res.matched && (res.consumed == input.length());
                if (!res.matched)
                    ctx.reset();
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
                    auto res = ast_type::match(input, {start_pos, str_length}, ctx);
                    if (res)
                    {
                        std::get<0>(ctx.captures) = capture<0>{start_pos, res.consumed};
                        return regex_result{true, std::move(ctx.captures), input};
                    }
                    if constexpr (ast::has_atomic_group_v<ast_type>)
                        ctx.reset();
                    ++start_pos;
                }
                ctx.reset();
                return regex_result{false, std::move(ctx.captures), input};
            }

            template<string_like Str>
            [[nodiscard]] static constexpr auto find_all(Str const &input, std::size_t start_pos = 0) noexcept
            {
                return generator
                {
                    [&input = input, pos = start_pos]() mutable {
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
        [[nodiscard]] static constexpr decltype(auto) find_all(Str const &input, std::size_t start_pos = 0) noexcept
        {
            return with_flags<>::find_all(input, start_pos);
        }
    };
}
#endif //META_REGEX_BASE_HPP