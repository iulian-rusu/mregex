#ifndef META_REGEX_BASE_HPP
#define META_REGEX_BASE_HPP

#include "iterable_generator_adapter.hpp"
#include "ast/ast.hpp"
#include "context/match_context.hpp"
#include "regex_result.hpp"
#include "regex_token_generator.hpp"
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

        template<typename ... ExtraFlags>
        using with = regex_base<AST, Flags ..., ExtraFlags ...>;

        static constexpr std::size_t capture_count = ast_type::capture_count;

        constexpr regex_base() noexcept = default;

        /**
         * Matches a given input sequence agains the regex pattern.
         *
         * @param begin An iterator pointing to the start of the sequence
         * @param end   An iterator pointing to the end of the sequence
         * @return      A regex_result_view object
         */
        template<std::forward_iterator Iter>
        [[nodiscard]] static constexpr auto match(Iter begin, Iter end) noexcept
        {
            using match_context = create_match_context<Iter, ast_type, Flags ...>;

            match_context ctx{};
            std::size_t length = std::distance(begin, end);
            auto res = ast_type::match(begin, end, {begin, length}, ctx);
            res.matched = res.matched && (res.consumed == length);
            if (res.matched)
            {
                std::string_view matched_content{begin, end};
                std::get<0>(ctx.captures) = regex_capture_view<0>{matched_content};
            }
            else
            {
                ctx.clear_captures();
            }
            return result_type{res.matched, std::move(ctx.captures)};
        }

        /**
         * Searches for the first match of the pattern in the given input sequence.
         *
         * @param begin An iterator pointing to the start of the sequence
         * @param end   An iterator pointing to the end of the sequence
         * @return      A regex_result_view object
         */
        template<std::forward_iterator Iter>
        [[nodiscard]] static constexpr auto find_first(Iter begin, Iter end) noexcept
        {
            using match_context = create_match_context<Iter, ast_type, Flags ...>;

            match_context ctx{};
            std::size_t length = std::distance(begin, end);
            for (auto current = begin;; ++current)
            {
                if (auto res = ast_type::match(begin, end, {current, length}, ctx))
                {
                    std::string_view matched_content{current, current + res.consumed};
                    std::get<0>(ctx.captures) = regex_capture_view<0>{matched_content};
                    return result_type{true, std::move(ctx.captures)};
                }

                if (current != end)
                {
                    ctx.clear();
                    --length;
                }
                else
                {
                    break;
                }
            }
            return result_type{false, std::move(ctx.captures)};
        }

        /**
         * Finds all non-zero length matches of the pattern inside the input sequence.
         *
         * @param begin An iterator pointing to the start of the sequence
         * @param end   An iterator pointing to the end of the sequence
         * @return      An iterable generator that lazily evaluates subsequent matches
         */
        template<std::forward_iterator Iter>
        [[nodiscard]] static constexpr auto find_all(Iter begin, Iter end) noexcept
        {
            using match_context = create_match_context<Iter, ast_type, Flags ...>;

            regex_token_generator<match_context> generator{begin, end, begin};
            return iterable_generator_adapter{generator};
        }

        /**
         * Overloads for working with string-like inputs directly.
         * Some overloads have a specialized version for temporary objects
         * that are not trivially destructible. In this case, the method returns
         * an owning regex_result type to avoid invalid pointers.
         */

        template<string_like Str>
        [[nodiscard]] static constexpr auto match(Str const &input) noexcept
        {
            return match(input.begin(), input.end());
        }

        template<string_like Str>
        [[nodiscard]] static constexpr auto match(Str &&input) noexcept
        requires is_memory_owning_rvalue_v<Str &&>
        {
            return match(input.begin(), input.end()).own();
        }

        template<string_like Str>
        [[nodiscard]] static constexpr auto find_first(Str const &input) noexcept
        {
            return find_first(input.begin(), input.end());
        }

        template<string_like Str>
        [[nodiscard]] static constexpr auto find_first(Str &&input) noexcept
        requires is_memory_owning_rvalue_v<Str &&>
        {
            return find_first(input.begin(), input.end()).own();
        }

        template<string_like Str>
        [[nodiscard]] static constexpr auto find_all(Str &&input) noexcept
        {
            using iterator_type = decltype(input.begin());
            using match_context = create_match_context<iterator_type, ast_type, Flags ...>;

            regex_token_generator<match_context> generator{input.begin(), input.end(), input.begin()};
            return iterable_generator_adapter
            {
                [=, capture = make_universal_capture(std::forward<Str>(input))]() mutable {
                    return generator();
                }
            };
        }
    };
}
#endif //META_REGEX_BASE_HPP