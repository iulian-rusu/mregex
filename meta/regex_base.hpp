#ifndef META_REGEX_BASE_HPP
#define META_REGEX_BASE_HPP

#include "range_adapter.hpp"
#include "ast/ast.hpp"
#include "regex_context.hpp"
#include "utility/continuations.hpp"
#include "regex_result.hpp"
#include "regex_match_generator.hpp"
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
        /**
         * Metafunction used to add flags to the current regex type.
         */
        template<typename... ExtraFlags>
        using with = regex_base<AST, Flags ..., ExtraFlags ...>;

        using ast_type = AST;

        static constexpr std::size_t capture_count = ast_type::capture_count;

        constexpr regex_base() noexcept = default;

        /**
         * Matches a given input sequence against the regex pattern.
         * The supplied iterator pair must form a valid range, otherwise
         * the behavior is undefined.
         *
         * @param begin An iterator pointing to the start of the sequence
         * @param end   An iterator pointing to the end of the sequence
         * @return      A regex_result_view object
         */
        template<std::forward_iterator Iter>
        [[nodiscard]] static constexpr auto match(Iter const begin, Iter const end) noexcept
        {
            using context_type = regex_context<Iter, ast_type, Flags ...>;
            using result_type = regex_result_view<ast_type::capture_count, Iter>;

            context_type ctx{};
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
        [[nodiscard]] static constexpr auto search(Iter const begin, Iter const end, Iter const from) noexcept
        {
            using context_type = regex_context<Iter, ast_type, Flags ...>;
            using result_type = regex_result_view<ast_type::capture_count, Iter>;

            context_type ctx{};
            for (Iter current = from;; ++current)
            {
                if (auto res = ast_type::match(begin, end, current, ctx, continuations<Iter>::epsilon))
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
         * Creates a range which contains all non-zero length matches of the
         * pattern inside the input sequence. The supplied iterator pair must
         * form a valid range, otherwise the behavior is undefined.
         *
         * @param begin An iterator pointing to the start of the sequence
         * @param end   An iterator pointing to the end of the sequence
         * @return      An iterable generator that lazily evaluates subsequent matches
         */
        template<std::forward_iterator Iter>
        [[nodiscard]] static constexpr auto range(Iter const begin, Iter const end) noexcept
        {
            using context_type = regex_context<Iter, ast_type, Flags ...>;

            return range_adapter{regex_match_generator<context_type>{begin, end, begin}};
        }

        /**
         * Overloads for working with string-like ranges directly.
         * Some overloads have a specialized version for temporary objects
         * that are not trivially destructible. In this case, the method returns
         * an owning regex_result type to avoid invalid pointers.
         */

        template<string_range S>
        [[nodiscard]] static constexpr auto match(S const &input) noexcept
        {
            return match(input.begin(), input.end());
        }

        template<string_range S>
        [[nodiscard]] static constexpr auto match(S &&input) noexcept
        requires is_memory_owning_rvalue_v<S &&>
        {
            return match(input.begin(), input.end()).own();
        }

        template<string_range S>
        [[nodiscard]] static constexpr auto search(S const &input) noexcept
        {
            return search(input.begin(), input.cend(), input.begin());
        }

        template<string_range S>
        [[nodiscard]] static constexpr auto search(S &&input) noexcept
        requires is_memory_owning_rvalue_v<S &&>
        {
            return search(input.begin(), input.end(), input.begin()).own();
        }

        template<string_range S>
        [[nodiscard]] static constexpr auto range(S &&input) noexcept
        {
            using iterator_type = decltype(input.begin());
            using context_type = regex_context<iterator_type, ast_type, Flags ...>;

            regex_match_generator<context_type> generator{input.begin(), input.end(), input.begin()};
            return range_adapter
            {
                [=, capture = make_universal_capture(std::forward<S>(input))]() mutable {
                    return generator();
                }
            };
        }
    };
}
#endif //META_REGEX_BASE_HPP