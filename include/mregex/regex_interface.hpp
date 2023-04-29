#ifndef MREGEX_REGEX_INTERFACE_HPP
#define MREGEX_REGEX_INTERFACE_HPP

#include <mregex/ast/traits.hpp>
#include <mregex/utility/input_range_adapter.hpp>
#include <mregex/regex_context.hpp>
#include <mregex/regex_flags.hpp>
#include <mregex/regex_result.hpp>
#include <mregex/regex_result_generator.hpp>

namespace meta
{
    /**
     * Class that provides an interface for all regex-like types.
     *
     * @tparam AST      The Abstract Syntax Tree of the regex
     * @tparam Flags    Optional flags for matching
     */
    template<typename AST, typename... Flags>
    struct regex_interface
    {
        using ast_type = AST;
        using flags = regex_flag_accessor<Flags ...>;

        template<std::forward_iterator Iter>
        using context_type = regex_context<regex_interface<ast_type, Flags ...>, Iter>;

        template<std::forward_iterator Iter>
        using result_view_type = regex_result_view<Iter, ast::capture_name_spec_t<ast_type>>;

        using result_type = regex_result<ast::capture_name_spec_t<ast_type>>;
        using match_method = regex_match_method<regex_interface<ast_type, Flags ...>>;
        using match_prefix_method = regex_match_prefix_method<regex_interface<ast_type, Flags ...>>;
        using search_method = regex_search_method<regex_interface<ast_type, Flags ...>>;

        template<std::forward_iterator Iter>
        using tokenizer_type = regex_result_generator<match_prefix_method, Iter>;

        template<std::forward_iterator Iter>
        using searcher_type = regex_result_generator<search_method, Iter>;

        template<std::forward_iterator Iter>
        using token_range_type = input_range_adapter<tokenizer_type<Iter>>;

        template<std::forward_iterator Iter>
        using match_range_type = input_range_adapter<searcher_type<Iter>>;

        /**
         * Metafunction used to add flags to the current regex type.
         *
         * @tparam ExtraFlags   The new flags to be added
         */
        template<typename... ExtraFlags>
        using with_flags = regex_interface<ast_type, Flags ..., ExtraFlags ...>;

        /**
         * Metafunction used to clear all flags from the current regex type.
         */
        using without_flags = regex_interface<ast_type>;

        static constexpr std::size_t capture_count = ast::capture_count<ast_type>;

        constexpr regex_interface() noexcept = default;

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
         * The seaercher will skip over non-match positions to find the next match.
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
            context_type<Iter> ctx{};
            auto result = Method::invoke(begin, end, begin, ctx);
            return result_view_type<Iter>{std::move(ctx.captures), result.matched};
        }
    };
}
#endif //MREGEX_REGEX_INTERFACE_HPP