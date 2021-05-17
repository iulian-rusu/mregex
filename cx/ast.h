#ifndef CX_AST_H
#define CX_AST_H

#include "utility.h"
#include "regex_result.h"

/**
 * File with the building blocks of the Abstract Syntax Tree
 */
namespace cx
{
    struct terminal
    {
        static constexpr std::size_t capture_count = 0;
    };

    struct epsilon : terminal
    {
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
        match(auto &&input, std::size_t from, int, bool negated = false) noexcept
        {
            bool res = (C == input[from]) ^negated;
            return {res, res};
        }
    };

    template<typename First, typename ... Rest>
    struct sequence
    {
        static constexpr std::size_t capture_count = count_captures<First, Rest ...>::capture_count;

        static constexpr match_result
        match(auto &&input, std::size_t from, std::size_t max_chars, bool negated = false) noexcept
        {
            if constexpr (sizeof... (Rest) > 0)
            {
                std::size_t match_limit = max_chars;
                match_result matched{0, false};

                do
                {
                    matched = First::match(input, from, match_limit, negated);
                    if (!matched)
                        return {0, false};
                    if (auto rest_matched = sequence<Rest ...>::match(input, from + matched.count, max_chars - matched.count, negated))
                        return matched + rest_matched;
                    if (matched.count == 0 || match_limit == 0)
                        return {0, false};
                    match_limit = matched.count - 1;
                } while (matched);
                return matched;
            }
            return First::match(input, from, max_chars, negated);
        }
    };

    template<typename First, typename ... Rest>
    struct alternation
    {
        static constexpr std::size_t capture_count = count_captures<First, Rest ...>::capture_count;

        static constexpr match_result
        match(auto &&input, std::size_t from, std::size_t max_chars, bool negated = false) noexcept
        requires string_like<decltype(input)>
        {
            auto matched = First::match(input, from, max_chars, negated);
            if(matched && matched.count <= max_chars)
            {
                return matched;
            }
            if constexpr (sizeof... (Rest) > 0)
            {
                return alternation<Rest ...>::match(input, from, max_chars, negated);
            }
            return {0, false};
        }
    };

    template<typename First, typename ... Rest>
    struct star
    {
        static constexpr std::size_t capture_count = count_captures<First, Rest ...>::capture_count;

        static constexpr match_result
        match(auto &&input, std::size_t from, std::size_t max_chars, bool negated = false) noexcept
        {
            if (max_chars == 0)
                return {0, true};

            if (auto matched = First::match(input, from, max_chars, negated))
            {
                if (matched.count > max_chars)
                    return {0, true};
                if constexpr (sizeof... (Rest) > 0)
                {
                    matched += star<Rest ...>::match(input, from + matched.count, max_chars - matched.count, negated);
                }
                return matched + match(input, from + matched.count, max_chars - matched.count, negated);
            }
            return {0, true};
        }
    };

    template<typename T>
    using optional = alternation<T, epsilon>;

    template<typename T>
    using plus = sequence<T, star<T>>;

    struct digit : terminal
    {
        static constexpr match_result
        match(auto &&input, std::size_t from, std::size_t, bool negated = false) noexcept
        {
            bool res = ('0' <= input[from] && input[from] <= '9') ^negated;
            return {res, res};
        }
    };

    struct lower : terminal
    {
        static constexpr match_result
        match(auto &&input, std::size_t from, std::size_t, bool negated = false) noexcept
        {
            bool res = ('a' <= input[from] && input[from] <= 'z') ^negated;
            return {res, res};
        }
    };

    struct upper : terminal
    {
        static constexpr match_result
        match(auto &&input, std::size_t from, std::size_t, bool negated = false) noexcept
        {
            bool res = ('A' <= input[from] && input[from] <= 'Z') ^negated;
            return {res, res};
        }
    };

    struct alnum : terminal
    {
        static constexpr match_result
        match(auto &&input, std::size_t from, std::size_t max_chars, bool negated = false) noexcept
        {
            if (auto res = digit::match(input, from, max_chars, negated)) return res;
            if (auto res = lower::match(input, from, max_chars, negated)) return res;
            return upper::match(input, from, max_chars, negated);
        }
    };

    struct word : terminal
    {
        static constexpr match_result
        match(auto &&input, std::size_t from, std::size_t max_chars, bool negated = false) noexcept
        {
            if (auto res = alnum::match(input, from, max_chars, negated))
                return res;
            bool res = (input[from] == '_') ^negated;
            return {res, res};
        }
    };

    struct whitespace : terminal
    {
        static constexpr match_result
        match(auto &&input, std::size_t from, std::size_t, bool negated = false) noexcept
        {
            bool res = (input[from] == ' ' || input[from] == '\t' ||
                        input[from] == '\n' || input[from] == '\r' ||
                        input[from] == '\f' || input[from] == '\x0B') ^negated;
            return {res, res};
        }
    };


    struct hexa : terminal
    {
        static constexpr match_result
        match(auto &&input, std::size_t from, std::size_t max_chars, bool negated = false) noexcept
        {
            bool res = (digit::match(input, from, max_chars, negated).count ||
                        'A' <= input[from] && input[from] <= 'F' ||
                        'a' <= input[from] && input[from] <= 'f') ^negated;
            return {res, res};
        }
    };

    struct wildcard : terminal
    {
        static constexpr match_result
        match(auto &&input, std::size_t from, std::size_t, bool negated = false) noexcept
        {
            bool res = (input[from] != '\n' && input[from] != '\r') ^ negated;
            return {res, res};
        }
    };

    template<typename S>
    struct negated
    {
        static constexpr std::size_t capture_count = S::capture_count;

        static constexpr match_result
        match(auto &&input, std::size_t from, std::size_t max_chars, bool negated = false) noexcept
        {
            return S::match(input, from, max_chars, !negated);
        }
    };

    template<typename S>
    struct capturing
    {
        static constexpr std::size_t capture_count = 1 + S::capture_count;

        static constexpr match_result
        match(auto &&input, std::size_t from, std::size_t max_chars, bool negated = false) noexcept
        {
            return S::match(input, from, max_chars, negated);
        }
    };
}
#endif //CX_AST_H
