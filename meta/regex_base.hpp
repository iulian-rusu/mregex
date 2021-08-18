#ifndef META_REGEX_BASE_HPP
#define META_REGEX_BASE_HPP

#include "generator.hpp"
#include "ast/ast_traits.hpp"
#include "ast/ast.hpp"
#include "context/match_context.hpp"
#include "regex_result.hpp"
#include "utility/universal_capture.hpp"

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
        using result_type = regex_result_view<ast_type::capture_count>;

        static constexpr std::size_t capture_count = ast_type::capture_count;
        static constexpr std::size_t atomic_count = ast_type::atomic_count;

        constexpr regex_base() noexcept = default;

        template<typename ... ExtraFlags>
        struct with_flags
        {
            using match_context = create_match_context<ast_type, Flags ..., ExtraFlags ...>;

            template<string_like Str>
            [[nodiscard]] static constexpr auto match(Str const &input) noexcept
            {
                match_context ctx{};
                auto res = ast_type::match(input, {0, input.length()}, ctx);
                res.matched = res.matched && (res.consumed == input.length());
                if (!res.matched)
                {
                    ctx.clear();
                }
                else
                {
                    std::string_view matched_content{input.cbegin(), input.cbegin() + res.consumed};
                    std::get<0>(ctx.captures) = regex_capture_view<0>{matched_content};
                }
                return result_type{res.matched, std::move(ctx.captures)};
            }

            template<string_like Str>
            [[nodiscard]] static constexpr auto find_first(Str const &input, std::size_t start_pos = 0) noexcept
            {
                match_context ctx{};
                std::size_t const str_length = input.length();
                do
                {
                    auto res = ast_type::match(input, {start_pos, str_length - start_pos}, ctx);
                    if (res)
                    {
                        auto start_iter = input.cbegin() + start_pos;
                        std::string_view matched_content{start_iter, start_iter + res.consumed};
                        std::get<0>(ctx.captures) = regex_capture_view<0>{matched_content};
                        return result_type{true, std::move(ctx.captures)};
                    }

                    if constexpr (ast::has_atomic_group_v<ast_type>)
                        ctx.clear();

                    ++start_pos;
                } while (start_pos < str_length);

                ctx.clear();
                return result_type{false, std::move(ctx.captures)};
            }

            template<string_like Str>
            [[nodiscard]] static constexpr auto find_all(Str &&input, std::size_t start_pos = 0) noexcept
            {
                return generator
                {
                    [input = make_universal_capture(std::forward<Str>(input)), pos = start_pos]() mutable {
                        auto const result = find_first(input.get(), pos);
                        pos += result.length() + 1;
                        return result;
                    }
                };
            }
        };

        template<string_like Str>
        [[nodiscard]] static constexpr auto match(Str const &input) noexcept
        {
            return with_flags<>::match(input);
        }

        template<string_like Str>
        [[nodiscard]] static constexpr auto find_first(Str const &input, std::size_t start_pos = 0) noexcept
        {
            return with_flags<>::find_first(input, start_pos);
        }

        template<string_like Str>
        [[nodiscard]] static constexpr auto find_all(Str &&input, std::size_t start_pos = 0) noexcept
        {
            return with_flags<>::find_all(std::forward<Str>(input), start_pos);
        }
    };
}
#endif //META_REGEX_BASE_HPP