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
        using match_context = create_match_context<ast_type, Flags ...>;

        template<typename ... ExtraFlags>
        using with = regex_base<AST, Flags ..., ExtraFlags ...>;

        static constexpr std::size_t capture_count = ast_type::capture_count;

        constexpr regex_base() noexcept = default;

        /**
         * Matches a given input string agains the regex pattern.
         *
         * @param input     A string-like object to be matched
         * @return          A regex_result_view object
         */
        template<string_like Str>
        [[nodiscard]] static constexpr auto match(Str const &input) noexcept
        {
            match_context ctx{};
            auto res = ast_type::match(input, {0, input.length()}, ctx);
            res.matched = res.matched && (res.consumed == input.length());
            if (res.matched)
            {
                std::string_view matched_content{input.cbegin(), input.cbegin() + res.consumed};
                std::get<0>(ctx.captures) = regex_capture_view<0>{matched_content};
            }
            else
            {
                ctx.clear_captures();
            }
            return result_type{res.matched, 0, std::move(ctx.captures)};
        }

        /**
         * Searches for the first match of the pattern in the given input string.
         *
         * @param input         A string-like object to be searched
         * @param start_pos     The starting offset of the search inside the input
         * @return              A regex_result_view object
         */
        template<string_like Str>
        [[nodiscard]] static constexpr auto find_first(Str const &input, std::size_t start_pos = 0) noexcept
        {
            match_context ctx{};
            std::size_t const str_length = input.length();
            do
            {
                if (auto res = ast_type::match(input, {start_pos, str_length - start_pos}, ctx))
                {
                    auto start_iter = input.cbegin() + start_pos;
                    std::string_view matched_content{start_iter, start_iter + res.consumed};
                    std::get<0>(ctx.captures) = regex_capture_view<0>{matched_content};
                    return result_type{true, start_pos, std::move(ctx.captures)};
                }

                if constexpr (ast::has_atomic_group_v<ast_type>)
                    ctx.clear_atomic_state();
                if constexpr (flags<match_context>::cache)
                    ctx.clear_cache();

                ++start_pos;
            } while (start_pos <= str_length);

            ctx.clear_captures();
            return result_type{false, start_pos, std::move(ctx.captures)};
        }

        /**
         * Finds all matches of the pattern inside the input string.
         *
         * @param input         The input string-like object to be searched
         * @param start_pos     The starting offset of the search inside the input
         * @return              A generator that lazily evaluates subsequent matches
         */
        template<string_like Str>
        [[nodiscard]] static constexpr auto find_all(Str &&input, std::size_t start_pos = 0) noexcept
        {
            return generator
            {
                [input = make_universal_capture(std::forward<Str>(input)), pos = start_pos]() mutable {
                    auto const result = find_first(input.get(), pos);
                    pos = result.end();
                    return result;
                }
            };
        }

        /**
         * Overloads for working with rvalues to temporary objects that will deallocate their memory
         * upon expiring. These methods return owning regex_result objects.
         */

        template<string_like Str>
        [[nodiscard]] static constexpr auto match(Str &&input) noexcept
        requires is_memory_owning_rvalue_v<Str &&>
        {
            return match(input).own();
        }

        template<string_like Str>
        [[nodiscard]] static constexpr auto find_first(Str &&input, std::size_t start_pos = 0) noexcept
        requires is_memory_owning_rvalue_v<Str &&>
        {
            return find_first(input, start_pos).own();
        }
    };
}
#endif //META_REGEX_BASE_HPP