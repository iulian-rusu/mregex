#ifndef MREGEX_REGEX_INTERFACE_HPP
#define MREGEX_REGEX_INTERFACE_HPP

#include <mregex/ast/ast.hpp>
#include <mregex/utility/input_range_adapter.hpp>
#include <mregex/regex_match_generator.hpp>

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
        /**
         * Metafunction used to add flags to the current regex type.
         */
        template<typename... ExtraFlags>
        using with = regex_interface<AST, Flags ..., ExtraFlags ...>;

        using ast_type = AST;

        template<std::forward_iterator Iter>
        using context_type = regex_context<Iter, ast_type, Flags ...>;

        template<std::forward_iterator Iter>
        using result_type = typename context_type<Iter>::result_type ;

        static constexpr std::size_t capture_count = ast_type::capture_count;

        constexpr regex_interface() noexcept = default;

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
            return invoke<match_method<ast_type>>(begin, end);
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
            return invoke<search_method<ast_type>>(begin, end);
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
            return regex_match_generator<context_type<Iter>>{begin, end};
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
        requires is_expiring_memory_owner<R &&>
        {
            return match(std::cbegin(input), std::cend(input)).as_memory_owner();
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
        requires is_expiring_memory_owner<R &&>
        {
            return search(std::cbegin(input), std::cend(input)).as_memory_owner();
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

        [[nodiscard]] static constexpr auto range(std::string_view input) noexcept
        {
            return range(std::cbegin(input), std::cend(input));
        }

        template<char_range R>
        [[nodiscard]] static constexpr auto range(R const &input) noexcept
        {
            return input_range_adapter{generator(input)};
        }

    private:
        template<typename Method, std::forward_iterator Iter>
        static constexpr auto invoke(Iter const begin, Iter const end) noexcept
        {
            context_type<Iter> ctx{};
            auto result = Method::invoke(begin, end, begin, ctx);
            return result_type<Iter>{std::move(ctx.captures), result.matched};
        }
    };

    template<typename Regex, std::forward_iterator Iter>
    using result_type_for = typename Regex::template result_type<Iter>;
}
#endif //MREGEX_REGEX_INTERFACE_HPP