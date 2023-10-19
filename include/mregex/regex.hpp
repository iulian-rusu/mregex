#ifndef MREGEX_REGEX_HPP
#define MREGEX_REGEX_HPP

#include <mregex/parser/parser.hpp>
#include <mregex/utility/input_range_adapter.hpp>
#include <mregex/match_result.hpp>
#include <mregex/match_result_generator.hpp>
#include <mregex/regex_flags.hpp>

namespace meta
{
    /**
     * Provides a high-level regex API for an Abstract Syntax Tree.
     *
     * @tparam AST      The Abstract Syntax Tree of the regular expression
     * @tparam Flags    Optional flags for matching
     */
    template<typename AST, regex_flag... Flags>
    struct regex_adapter;

    /**
     * Constructs a regular expression from a literal string.
     */
    template<static_string Pattern, regex_flag... Flags>
    requires (fail_on_syntax_error<Pattern>())
    using regex = regex_adapter<ast_of<Pattern>, Flags ...>;

    template<typename AST, regex_flag... Flags>
    struct regex_adapter
    {
        using self = regex_adapter<AST, Flags ...>;
        using ast_type = AST;
        using flags = regex_flag_sequence<Flags ...>;

        using match_method = regex_match_method<self>;
        using match_prefix_method = regex_match_prefix_method<self>;
        using search_method = regex_search_method<self>;

        template<std::forward_iterator Iter>
        using tokenizer_type = match_result_generator<match_prefix_method, Iter>;

        template<std::forward_iterator Iter>
        using searcher_type = match_result_generator<search_method, Iter>;

        template<std::forward_iterator Iter>
        using token_range_type = input_range_adapter<tokenizer_type<Iter>>;

        template<std::forward_iterator Iter>
        using match_range_type = input_range_adapter<searcher_type<Iter>>;

        /**
         * Metafunction used to add flags to the current regex type.
         *
         * @tparam ExtraFlags   The new flags to be added
         */
        template<regex_flag... ExtraFlags>
        using add_flags = regex_adapter<ast_type, Flags ..., ExtraFlags ...>;

        /**
         * Metafunction used to clear all flags from the current regex type.
         */
        using clear_flags = regex_adapter<ast_type>;

        static constexpr std::size_t capture_count = ast::capture_count<ast_type>;

        constexpr regex_adapter() noexcept = default;

        /**
         * Performs an exact match of the entire range against the pattern.
         * The supplied iterator pair must form a valid range, otherwise the behavior is undefined.
         *
         * @param begin An iterator pointing to the start of the input
         * @param end   An iterator pointing to the end of the input
         * @return      An object that holds the results of the match
         */
        template<std::forward_iterator Iter>
        [[nodiscard]] static constexpr auto match(Iter begin, Iter end) noexcept
        {
            return invoke<match_method>(begin, end);
        }

        /**
         * Matches the prefix of the given range against the pattern.
         * The supplied iterator pair must form a valid range, otherwise the behavior is undefined.
         *
         * @param begin An iterator pointing to the start of the input
         * @param end   An iterator pointing to the end of the input
         * @return      An object that holds the results of the match
         */
        template<std::forward_iterator Iter>
        [[nodiscard]] static constexpr auto match_prefix(Iter begin, Iter end) noexcept
        {
            return invoke<match_prefix_method>(begin, end);
        }

        /**
         * Searches the first match for the pattern inside the given range.
         * The supplied iterator pair must form a valid range, otherwise the behavior is undefined.
         *
         * @param begin An iterator pointing to the start of the input
         * @param end   An iterator pointing to the end of the input
         * @return      An object that holds the results of the search
         */
        template<std::forward_iterator Iter>
        [[nodiscard]] static constexpr auto search(Iter begin, Iter end) noexcept
        {
            return invoke<search_method>(begin, end);
        }

        /**
         * Returns a lazy tokenizer that yields all continuous matches inside the given range.
         * Continuous means that the tokenizer will stop at the first non-match position.
         * At most one empty token will be generated.
         * The supplied iterator pair must form a valid range, otherwise the behavior is undefined.
         *
         * @param begin An iterator pointing to the start of the sequence
         * @param end   An iterator pointing to the end of the sequence
         * @return      A functor that generates the tokens
         */
        template<std::forward_iterator Iter>
        [[nodiscard]] static constexpr auto tokenizer(Iter begin, Iter end) noexcept
        {
            return tokenizer_type<Iter>{begin, end};
        }

        /**
         * Returns a lazy searcher that yields all matches inside the given range.
         * The searcher will skip over non-match positions to find the next match.
         * At most one empty match will be generated.
         * The supplied iterator pair must form a valid range, otherwise the behavior is undefined.
         *
         * @param begin An iterator pointing to the start of the sequence
         * @param end   An iterator pointing to the end of the sequence
         * @return      A functor that searches for regex matches
         */
        template<std::forward_iterator Iter>
        [[nodiscard]] static constexpr auto searcher(Iter begin, Iter end) noexcept
        {
            return searcher_type<Iter>{begin, end};
        }

        /**
         * Returns a lazy view of all non-empty tokens inside the given range.
         * Tokens are continuous matches of the pattern, ending at the first non-match position.
         * The supplied iterator pair must form a valid range, otherwise the behavior is undefined.
         *
         * @param begin An iterator pointing to the start of the sequence
         * @param end   An iterator pointing to the end of the sequence
         * @return      An input range which contains all non-empty matches
         */
        template<std::forward_iterator Iter>
        [[nodiscard]] static constexpr auto tokenize(Iter begin, Iter end) noexcept
        {
            return token_range_type<Iter>{tokenizer(begin, end)};
        }

        /**
         * Returns a lazy view of all non-empty matches inside the given range.
         * The supplied iterator pair must form a valid range, otherwise the behavior is undefined.
         *
         * @param begin An iterator pointing to the start of the sequence
         * @param end   An iterator pointing to the end of the sequence
         * @return      An input range which contains all non-empty matches
         */
        template<std::forward_iterator Iter>
        [[nodiscard]] static constexpr auto find_all(Iter begin, Iter end) noexcept
        {
            return match_range_type<Iter>{searcher(begin, end)};
        }

        /**
         * Overloads for working with string-like ranges directly.
         */

        [[nodiscard]] static constexpr auto match(std::string_view input) noexcept
        {
            return match(std::cbegin(input), std::cend(input));
        }

        template<char_range Range>
        [[nodiscard]] static constexpr auto match(Range const &input) noexcept
        {
            return match(std::cbegin(input), std::cend(input));
        }

        template<char_range Range>
        [[nodiscard]] static constexpr auto match(Range &&input)
        requires is_expiring_memory_owner<Range &&>
        {
            return match(std::cbegin(input), std::cend(input)).as_memory_owner();
        }

        [[nodiscard]] static constexpr auto match_prefix(std::string_view input) noexcept
        {
            return match_prefix(std::cbegin(input), std::cend(input));
        }

        template<char_range Range>
        [[nodiscard]] static constexpr auto match_prefix(Range const &input) noexcept
        {
            return match_prefix(std::cbegin(input), std::cend(input));
        }

        template<char_range Range>
        [[nodiscard]] static constexpr auto match_prefix(Range &&input)
        requires is_expiring_memory_owner<Range &&>
        {
            return match_prefix(std::cbegin(input), std::cend(input)).as_memory_owner();
        }

        [[nodiscard]] static constexpr auto search(std::string_view input) noexcept
        {
            return search(std::cbegin(input), std::cend(input));
        }

        template<char_range Range>
        [[nodiscard]] static constexpr auto search(Range const &input) noexcept
        {
            return search(std::cbegin(input), std::cend(input));
        }

        template<char_range Range>
        [[nodiscard]] static constexpr auto search(Range &&input)
        requires is_expiring_memory_owner<Range &&>
        {
            return search(std::cbegin(input), std::cend(input)).as_memory_owner();
        }

        [[nodiscard]] static constexpr auto tokenizer(std::string_view input) noexcept
        {
            return tokenizer(std::cbegin(input), std::cend(input));
        }

        template<char_range Range>
        [[nodiscard]] static constexpr auto tokenizer(Range const &input) noexcept
        {
            return tokenizer(std::cbegin(input), std::cend(input));
        }

        [[nodiscard]] static constexpr auto searcher(std::string_view input) noexcept
        {
            return searcher(std::cbegin(input), std::cend(input));
        }

        template<char_range Range>
        [[nodiscard]] static constexpr auto searcher(Range const &input) noexcept
        {
            return searcher(std::cbegin(input), std::cend(input));
        }

        [[nodiscard]] static constexpr auto tokenize(std::string_view input) noexcept
        {
            return tokenize(std::cbegin(input), std::cend(input));
        }

        template<char_range Range>
        [[nodiscard]] static constexpr auto tokenize(Range const &input) noexcept
        {
            return tokenize(std::cbegin(input), std::cend(input));
        }

        [[nodiscard]] static constexpr auto find_all(std::string_view input) noexcept
        {
            return find_all(std::cbegin(input), std::cend(input));
        }

        template<char_range Range>
        [[nodiscard]] static constexpr auto find_all(Range const &input) noexcept
        {
            return find_all(std::cbegin(input), std::cend(input));
        }

    private:
        template<typename Method, std::forward_iterator Iter>
        static constexpr auto invoke(Iter begin, Iter end) noexcept
        {
            regex_match_context<self, Iter> ctx{};
            auto result = Method::invoke(begin, end, begin, ctx);
            return match_result_view<self, Iter>{std::move(ctx.captures), result.matched};
        }
    };
}
#endif //MREGEX_REGEX_HPP