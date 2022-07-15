#ifndef MREGEX_REGEX_BASE_HPP
#define MREGEX_REGEX_BASE_HPP

#include <mregex/ast/ast.hpp>
#include <mregex/utility/continuations.hpp>
#include <mregex/utility/input_range_adapter.hpp>
#include <mregex/utility/universal_capture.hpp>
#include <mregex/regex_context.hpp>
#include <mregex/regex_match_generator.hpp>
#include <mregex/regex_result.hpp>

namespace ranges = std::ranges;

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
         * @return      An object that holds the results of the match
         */
        template<std::forward_iterator Iter>
        [[nodiscard]] static constexpr auto match(Iter const begin, Iter const end) noexcept
        {
            using context_type = regex_context<Iter, ast_type, Flags ...>;
            using result_type = typename context_type::result_type;

            context_type ctx{};
            auto res = ast_type::match(begin, end, begin, ctx, continuations<Iter>::equals(end));
            if (res.matched)
                std::get<0>(ctx.captures) = regex_capture_view<Iter>{begin, end};
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
         * @return      An object that holds the results of the search
         */
        template<std::forward_iterator Iter>
        [[nodiscard]] static constexpr auto search(Iter const begin, Iter const end) noexcept
        {
            using context_type = regex_context<Iter, ast_type, Flags ...>;
            using result_type = typename context_type::result_type;

            context_type ctx{};
            for (Iter it = begin;; ++it)
            {
                if (auto match = ast_type::match(begin, end, it, ctx, continuations<Iter>::epsilon))
                {
                    std::get<0>(ctx.captures) = regex_capture_view<Iter>{it, match.end};
                    return result_type{true, std::move(ctx.captures)};
                }

                if (it == end)
                    break;
                ctx.clear();
            }
            return result_type{false, std::move(ctx.captures)};
        }

        /**
         * Creates a lazy generator that yields non-empty matches in the given
         * input sequence. The supplied iterator pair must form a valid range,
         * otherwise the behavior is undefined.
         *
         * @param begin An iterator pointing to the start of the sequence
         * @param end   An iterator pointing to the end of the sequence
         * @return      A functor that generates regex matches
         */
        template<std::forward_iterator Iter>
        [[nodiscard]] static constexpr auto generator(Iter const begin, Iter const end) noexcept
        {
            using context_type = regex_context<Iter, ast_type, Flags ...>;

            return regex_match_generator<context_type>{begin, end};
        }

        /**
         * Creates a range which contains all non-empty length matches of the
         * pattern inside the input sequence. The supplied iterator pair must
         * form a valid range, otherwise the behavior is undefined.
         *
         * @param begin An iterator pointing to the start of the sequence
         * @param end   An iterator pointing to the end of the sequence
         * @return      A range whose elements can be accessed with input iterators
         */
        template<std::forward_iterator Iter>
        [[nodiscard]] static constexpr auto range(Iter const begin, Iter const end) noexcept
        {
            return input_range_adapter{generator(begin, end)};
        }

        /**
         * Overloads for working with string-like ranges directly.
         * Some overloads have a specialized version for temporary objects
         * that are not trivially destructible. In this case, the method returns
         * an owning regex_result type to avoid invalid pointers.
         */

        [[nodiscard]] static constexpr auto match(std::string_view input) noexcept
        {
            return match(std::cbegin(input), std::cend(input));
        }

        template<char_range R>
        [[nodiscard]] static constexpr auto match(R const &input) noexcept
        {
            return match(std::cbegin(input), std::cend(input));
        }

        template<char_range R>
        [[nodiscard]] static constexpr auto match(R &&input) noexcept
        requires is_expiring_memory_owner_v<R &&>
        {
            return match(std::cbegin(input), std::cend(input)).own();
        }

        [[nodiscard]] static constexpr auto search(std::string_view input) noexcept
        {
            return search(std::cbegin(input), std::cend(input));
        }

        template<char_range R>
        [[nodiscard]] static constexpr auto search(R const &input) noexcept
        {
            return search(std::cbegin(input), std::cend(input));
        }

        template<char_range R>
        [[nodiscard]] static constexpr auto search(R &&input) noexcept
        requires is_expiring_memory_owner_v<R &&>
        {
            return search(std::cbegin(input), std::cend(input)).own();
        }

        [[nodiscard]] static constexpr auto generator(std::string_view input) noexcept
        {
            return generator(std::cbegin(input), std::cend(input));
        }

        template<char_range R>
        [[nodiscard]] static constexpr auto generator(R const &input) noexcept
        {
            return generator(std::cbegin(input), std::cend(input));
        }

        template<char_range R>
        [[nodiscard]] static constexpr auto generator(R &&input) noexcept
        requires is_expiring_memory_owner_v<R &&>
        {
            using iterator_type = decltype(std::cbegin(input));
            using context_type = regex_context<iterator_type, ast_type, Flags ...>;

            auto begin = std::cbegin(input);
            auto end = std::cend(input);
            regex_match_generator<context_type> generator{begin, end};
            return [=, cap = universal_capture{std::forward<R>(input)}]() mutable {
                return generator();
            };
        }

        [[nodiscard]] static constexpr auto range(std::string_view input) noexcept
        {
            return range(std::cbegin(input), std::cend(input));
        }

        template<char_range R>
        [[nodiscard]] static constexpr auto range(R &&input) noexcept
        {
            return input_range_adapter{generator(std::forward<R>(input))};
        }
    };
}
#endif //MREGEX_REGEX_BASE_HPP