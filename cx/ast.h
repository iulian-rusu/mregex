#ifndef CX_AST_H
#define CX_AST_H

#include "utility.h"
#include "regex_result.h"

/**
 * File with the building blocks of the Abstract Syntax Tree
 */
namespace cx
{
    template<typename First, typename ... Rest>
    struct sequence
    {
        static constexpr std::size_t capture_count = count_captures<First, Rest ...>::capture_count;

        template<std::size_t N> static constexpr match_result
        match(auto const &input, std::size_t from, std::size_t max_chars, capture_storage<N> &captures, bool negated = false) noexcept
        {
            if constexpr (sizeof... (Rest) > 0)
            {
                std::size_t match_limit = max_chars;
                do
                {
                    auto first_match = First::template match<N>(input, from, match_limit, captures, negated);
                    if (!first_match)
                        return {0, false};
                    if (auto rest_matched = sequence<Rest ...>::template match<N>(
                            input, from + first_match.count, max_chars - first_match.count, captures, negated))
                        return first_match + rest_matched;
                    if (first_match.count == 0 || match_limit == 0)
                        return {0, false};
                    match_limit = first_match.count - 1;
                } while (true);
            }
            return First::template match<N>(input, from, max_chars, captures, negated);
        }
    };

    template<typename First, typename ... Rest>
    struct alternation
    {
        static constexpr std::size_t capture_count = count_captures<First, Rest ...>::capture_count;

        template<std::size_t N> static constexpr match_result
        match(auto const &input, std::size_t from, std::size_t max_chars, capture_storage<N> &captures, bool negated = false) noexcept
        {
            auto first_match = First::template match<N>(input, from, max_chars, captures, false);
            if(first_match && first_match.count <= max_chars)
            {
                first_match.matched ^= negated;
                return first_match;
            }
            if constexpr (sizeof... (Rest) > 0)
            {
                return alternation<Rest ...>::template match<N>(input, from, max_chars, captures, negated);
            }
            // if negated == true and nothing was matched before, match one character (in case of negated sets)
            return {negated, negated};
        }
    };

    template<typename Inner>
    struct star
    {
        static constexpr std::size_t capture_count = Inner::capture_count;

        template<std::size_t N> static constexpr match_result
        match(auto const &input, std::size_t from, std::size_t max_chars, capture_storage<N> &captures, bool negated = false) noexcept
        {
            if (max_chars == 0)
                return {0, true};

            if (auto inner_match = Inner::template match<N>(input, from, max_chars, captures, negated))
            {
                if (inner_match.count > max_chars)
                    return {0, true};
                return inner_match + match<N>(input, from + inner_match.count, max_chars - inner_match.count, captures, negated);
            }
            return {0, true};
        }
    };

    struct terminal
    {
        static constexpr std::size_t capture_count = 0;
    };

    struct epsilon : terminal
    {
        // epsilon always matches anything and consumes no characters
        template<std::size_t N>
        static constexpr match_result match(auto const &, std::size_t, std::size_t, capture_storage<N> &, ...) noexcept
        {
            return {0, true};
        }
    };

    template<auto C>
    struct character : terminal
    {
        template<std::size_t N> static constexpr match_result
        match(auto const &input, std::size_t from, std::size_t, capture_storage<N> &, bool negated = false) noexcept
        {
            bool res = (from < input.length() && C == input[from]) ^negated;
            return {res, res};
        }
    };

    struct whitespace : terminal
    {
        template<std::size_t N> static constexpr match_result
        match(auto const &input, std::size_t from, std::size_t, capture_storage<N> &, bool negated = false) noexcept
        {
            bool res = (from < input.length() &&
                        (input[from] == ' ' || input[from] == '\t' ||
                         input[from] == '\n' || input[from] == '\r' ||
                         input[from] == '\f' || input[from] == '\x0B')) ^negated;
            return {res, res};
        }
    };

    struct wildcard : terminal
    {
        template<std::size_t N> static constexpr match_result
        match(auto const &input, std::size_t from, std::size_t, capture_storage<N> &, bool negated = false) noexcept
        {
            bool res = (from < input.length() && input[from] != '\n' && input[from] != '\r') ^ negated;
            return {res, res};
        }
    };

    template<auto A, auto B>
    struct range : terminal
    {
        static_assert(A < B, "invalid range parameters");

        template<std::size_t N> static constexpr match_result
        match(auto const &input, std::size_t from, int, capture_storage<N> &, bool negated = false) noexcept
        {
            bool res = (from < input.length() && A <= input[from] && input[from] <= B) ^negated;
            return {res, res};
        }
    };

    template<typename T>
    using optional = alternation<T, epsilon>;

    template<typename T>
    using plus = sequence<T, star<T>>;

    using digit = range<'0', '9'>;

    using lower = range<'a', 'z'>;

    using upper = range<'A', 'Z'>;

    using alnum = alternation<range<'A', 'z'>, digit>;

    using word = alternation<range<'A', 'z'>, digit, character<'_'>>;

    using hexa = alternation<digit, range<'a', 'f'>, range<'A', 'F'>>;

    // decorators for AST nodes
    template<typename Inner>
    struct negated
    {
        static constexpr std::size_t capture_count = Inner::capture_count;

        template<std::size_t N> static constexpr match_result
        match(auto const &input, std::size_t from, std::size_t max_chars, capture_storage<N> &captures, bool negated = false) noexcept
        {
            return Inner::template match<N>(input, from, max_chars, captures, !negated);
        }
    };


    template<std::size_t ID, typename Inner>
    struct capturing
    {
        static constexpr std::size_t capture_count = 1 + Inner::capture_count;

        template<std::size_t N> static constexpr match_result
        match(auto const &input, std::size_t from, std::size_t max_chars, capture_storage<N> &captures, bool negated = false) noexcept
        {
            auto inner_match = Inner::template match<N>(input, from, max_chars, captures, negated);
            std::get<ID>(captures) = capture<ID>{from, inner_match.count};
            return inner_match;
        }
    };
}
#endif //CX_AST_H
