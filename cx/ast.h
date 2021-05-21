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

        static constexpr match_result
        match(auto const &input, std::size_t from, std::size_t max_chars, bool negated = false) noexcept
        {
            if constexpr (sizeof... (Rest) > 0)
            {
                std::size_t match_limit = max_chars;
                match_result first_match{0, false};

                do
                {
                    first_match = First::match(input, from, match_limit, negated);
                    if (!first_match)
                        return {0, false};
                    if (auto rest_matched = sequence<Rest ...>::match(input, from + first_match.count, max_chars - first_match.count, negated))
                        return first_match + rest_matched;
                    if (first_match.count == 0 || match_limit == 0)
                        return {0, false};
                    match_limit = first_match.count - 1;
                } while (first_match);
                return first_match;
            }
            return First::match(input, from, max_chars, negated);
        }
    };

    template<typename First, typename ... Rest>
    struct alternation
    {
        static constexpr std::size_t capture_count = count_captures<First, Rest ...>::capture_count;

        static constexpr match_result
        match(auto const &input, std::size_t from, std::size_t max_chars, bool negated = false) noexcept
        {
            auto first_match = First::match(input, from, max_chars, negated);
            if(first_match && first_match.count <= max_chars)
            {
                return first_match;
            }
            if constexpr (sizeof... (Rest) > 0)
            {
                return alternation<Rest ...>::match(input, from, max_chars, negated);
            }
            return {0, false};
        }
    };

    template<typename Inner>
    struct star
    {
        static constexpr std::size_t capture_count = Inner::capture_count;

        static constexpr match_result
        match(auto const &input, std::size_t from, std::size_t max_chars, bool negated = false) noexcept
        {
            if (max_chars == 0)
                return {0, true};

            if (auto inner_match = Inner::match(input, from, max_chars, negated))
            {
                if (inner_match.count > max_chars)
                    return {0, true};
                return inner_match + match(input, from + inner_match.count, max_chars - inner_match.count, negated);
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
        static constexpr match_result
        match(...) noexcept
        {
            return {0, true};
        }
    };

    template<auto C>
    struct character : terminal
    {
        static constexpr match_result
        match(auto const &input, std::size_t from, int, bool negated = false) noexcept
        {
            bool res = (from < input.length() && C == input[from]) ^negated;
            return {res, res};
        }
    };

    struct whitespace : terminal
    {
        static constexpr match_result
        match(auto const &input, std::size_t from, std::size_t, bool negated = false) noexcept
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
        static constexpr match_result
        match(auto const &input, std::size_t from, std::size_t, bool negated = false) noexcept
        {
            bool res = (from < input.length() && input[from] != '\n' && input[from] != '\r') ^ negated;
            return {res, res};
        }
    };

    template<auto A, auto B>
    struct range : terminal
    {
        static_assert(A < B, "invalid range parameters");
        static constexpr match_result
        match(auto const &input, std::size_t from, int, bool negated = false) noexcept
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

        static constexpr match_result
        match(auto const &input, std::size_t from, std::size_t max_chars, bool negated = false) noexcept
        {
            return Inner::match(input, from, max_chars, !negated);
        }
    };

    template<typename Inner>
    struct capturing
    {
        static constexpr std::size_t capture_count = 1 + Inner::capture_count;

        static constexpr match_result
        match(auto const &input, std::size_t from, std::size_t max_chars, bool negated = false) noexcept
        {
            return Inner::match(input, from, max_chars, negated);
        }
    };
}
#endif //CX_AST_H
