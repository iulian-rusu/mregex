#ifndef META_REGEX_BASE_HPP
#define META_REGEX_BASE_HPP

#include "iterable_generator_adapter.hpp"
#include "ast/ast.hpp"
#include "regex_context.hpp"
#include "continuation.hpp"
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
    template<typename AST, typename... Flags>
    struct regex_base
    {
        using ast_type = AST;

        template<typename... ExtraFlags>
        using with = regex_base<AST, Flags ..., ExtraFlags ...>;

        static constexpr std::size_t capture_count = ast_type::capture_count;

        constexpr regex_base() noexcept = default;

        /**
         * Matches a given input sequence agains the regex pattern.
         * The supplied iterator pair must form a valid range, otherwise
         * the behavior is undefined.
         *
         * @param begin An iterator pointing to the start of the sequence
         * @param end   An iterator pointing to the end of the sequence
         * @return      A regex_result_view object
         */
        template<std::forward_iterator Iter>
        [[nodiscard]] static constexpr auto match(Iter begin, Iter end) noexcept
        {
            using match_context = create_regex_context<Iter, ast_type, Flags ...>;
            using result_type = regex_result_view<ast_type::capture_count, Iter>;

            match_context ctx{};
            auto res = ast_type::match(begin, end, begin, ctx, continuations<Iter>::equals(end));
            if (res.matched)
                std::get<0>(ctx.captures) = regex_capture_view<0, Iter>{begin, end};
            else
                ctx.clear();
            return result_type{res.matched, std::move(ctx.captures)};
        }

        /**
         * Searches for the first match of the pattern in the given input sequence.
         * The supplied iterator pair must form a valid range, otherwise
         * the behavior is undefined.
         *
         * @param begin An iterator pointing to the start of the sequence
         * @param end   An iterator pointing to the end of the sequence
         * @param from  The iterator to start searching from
         * @return      A regex_result_view object
         */
        template<std::forward_iterator Iter>
        [[nodiscard]] static constexpr auto find_first(Iter begin, Iter end, Iter from) noexcept
        {
            using match_context = create_regex_context<Iter, ast_type, Flags ...>;
            using result_type = regex_result_view<ast_type::capture_count, Iter>;

            match_context ctx{};
            for (auto current = from;; ++current)
            {
                if (auto res = ast_type::match(begin, end, current, ctx, continuations<Iter>::empty))
                {
                    std::get<0>(ctx.captures) = regex_capture_view<0, Iter>{current, res.end};
                    return result_type{true, std::move(ctx.captures)};
                }

                if (current != end)
                    ctx.clear();
                else
                    break;
            }
            return result_type{false, std::move(ctx.captures)};
        }

        /**
         * Finds all non-zero length matches of the pattern inside the input sequence.
         * The supplied iterator pair must form a valid range, otherwise
         * the behavior is undefined.
         *
         * @param begin An iterator pointing to the start of the sequence
         * @param end   An iterator pointing to the end of the sequence
         * @return      An iterable generator that lazily evaluates subsequent matches
         */
        template<std::forward_iterator Iter>
        [[nodiscard]] static constexpr auto find_all(Iter begin, Iter end) noexcept
        {
            using match_context = create_regex_context<Iter, ast_type, Flags ...>;

            regex_token_generator<match_context> generator{begin, end, begin};
            return iterable_generator_adapter{generator};
        }

        /**
         * Overloads for working with string-like inputs directly.
         * Some overloads have a specialized version for temporary objects
         * that are not trivially destructible. In this case, the method returns
         * an owning regex_result type to avoid invalid pointers.
         */

        template<string_like S>
        [[nodiscard]] static constexpr auto match(S const &input) noexcept
        {
            return match(input.begin(), input.end());
        }

        template<string_like S>
        [[nodiscard]] static constexpr auto match(S &&input) noexcept
        requires is_memory_owning_rvalue_v<S &&>
        {
            return match(input.begin(), input.end()).own();
        }

        template<string_like S>
        [[nodiscard]] static constexpr auto find_first(S const &input) noexcept
        {
            return find_first(input.begin(), input.end(), input.begin());
        }

        template<string_like S>
        [[nodiscard]] static constexpr auto find_first(S &&input) noexcept
        requires is_memory_owning_rvalue_v<S &&>
        {
            return find_first(input.begin(), input.end(), input.begin()).own();
        }

        template<string_like S>
        [[nodiscard]] static constexpr auto find_all(S &&input) noexcept
        {
            using iterator_type = decltype(input.begin());
            using match_context = create_regex_context<iterator_type, ast_type, Flags ...>;

            regex_token_generator<match_context> generator{input.begin(), input.end(), input.begin()};
            return iterable_generator_adapter
            {
                [=, capture = make_universal_capture(std::forward<S>(input))]() mutable {
                    return generator();
                }
            };
        }
    };
}
#endif //META_REGEX_BASE_HPP