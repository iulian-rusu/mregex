#ifndef CX_AST_H
#define CX_AST_H

#include "capture_counter.h"
#include "regex_result.h"
#include "match_result.h"

/**
 * File with the building blocks of the Abstract Syntax Tree
 */
namespace cx
{
    struct match_params
    {
        std::size_t from{};
        std::size_t max_chars{};
        bool negated = false;
    };

    template<typename First, typename ... Rest>
    struct sequence
    {
        static constexpr std::size_t capture_count = count_captures<First, Rest ...>::capture_count;

        template<std::size_t N>
        static constexpr match_result match(auto const &input, match_params mp, capture_storage<N> &captures) noexcept
        {
            std::size_t match_limit = mp.max_chars;
            do
            {
                auto first_match = First::template match<N>(input, {mp.from, match_limit, mp.negated}, captures);
                if (!first_match)
                    return {0, false};
                if (auto rest_matched = sequence<Rest ...>::template match<N>(
                        input, {mp.from + first_match.count, mp.max_chars - first_match.count, mp.negated},
                        captures))
                    return first_match + rest_matched;
                if (first_match.count == 0 || match_limit == 0)
                    return {0, false};
                match_limit = first_match.count - 1;
            } while (true);
        }
    };

    template<typename First>
    struct sequence<First>
    {
        static constexpr std::size_t capture_count = First::capture_count;

        template<std::size_t N>
        static constexpr match_result match(auto const &input, match_params mp, capture_storage<N> &captures) noexcept
        {
            return First::template match<N>(input, mp, captures);
        }
    };

    template<typename First, typename ... Rest>
    struct alternation
    {
        static constexpr std::size_t capture_count = count_captures<First, Rest ...>::capture_count;

        template<std::size_t N>
        static constexpr match_result match(auto const &input, match_params mp, capture_storage<N> &captures) noexcept
        {
            auto first_match = First::template match<N>(input, {mp.from, mp.max_chars, false}, captures);
            if (first_match && first_match.count <= mp.max_chars)
            {
                first_match.matched ^= mp.negated;
                return first_match;
            }
            if constexpr (sizeof... (Rest) > 0)
            {
                return alternation<Rest ...>::template match<N>(input, mp, captures);
            }
            // Default case if no node has matched in the entire alternation
            bool default_res = mp.from < input.length() && mp.negated;
            return {default_res, default_res};
        }
    };

    template<typename Inner>
    struct star
    {
        static constexpr std::size_t capture_count = Inner::capture_count;

        template<std::size_t N>
        static constexpr match_result match(auto const &input, match_params mp, capture_storage<N> &captures) noexcept
        {
            if (mp.max_chars == 0)
                return {0, true};

            if (auto inner_match = Inner::template match<N>(input, mp, captures))
            {
                if (inner_match.count > mp.max_chars)
                    return {0, true};
                return inner_match +
                       match<N>(input, {mp.from + inner_match.count, mp.max_chars - inner_match.count, mp.negated},
                                captures);
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
        // epsilon node always matches everything and consumes no characters
        template<std::size_t N>
        static constexpr match_result match(auto const &, match_params, capture_storage<N> &) noexcept
        {
            return {0, true};
        }
    };

    struct null : terminal
    {
        // null node never matches anything and consumes no characters
        template<std::size_t N>
        static constexpr match_result match(auto const &, match_params, capture_storage<N> &) noexcept
        {
            return {0, false};
        }
    };

    struct beginning : terminal
    {
        template<std::size_t N>
        static constexpr match_result match(auto const &, match_params mp, capture_storage<N> &) noexcept
        {
            return {0, mp.from == 0u};
        }
    };

    struct ending : terminal
    {
        template<std::size_t N>
        static constexpr match_result match(auto const &input, match_params mp, capture_storage<N> &) noexcept
        {
            return {0, mp.from == input.length()};
        }
    };

    template<auto C>
    struct character : terminal
    {
        template<std::size_t N>
        static constexpr match_result match(auto const &input, match_params mp, capture_storage<N> &) noexcept
        {
            bool res = (mp.from < input.length() && C == input[mp.from]) ^mp.negated;
            return {res, res};
        }
    };

    struct whitespace : terminal
    {
        template<std::size_t N>
        static constexpr match_result match(auto const &input, match_params mp, capture_storage<N> &) noexcept
        {
            auto from = mp.from;
            bool res = (from < input.length() &&
                        (input[from] == ' ' || input[from] == '\t' ||
                         input[from] == '\n' || input[from] == '\r' ||
                         input[from] == '\f' || input[from] == '\x0B')) ^mp.negated;
            return {res, res};
        }
    };

    struct wildcard : terminal
    {
        template<std::size_t N>
        static constexpr match_result match(auto const &input, match_params mp, capture_storage<N> &) noexcept
        {
            auto from = mp.from;
            bool res = (from < input.length() && input[from] != '\n' && input[from] != '\r') ^mp.negated;
            return {res, res};
        }
    };

    template<auto A, auto B>
    struct range : terminal
    {
        static_assert(A < B, "invalid range parameters");

        template<std::size_t N>
        static constexpr match_result match(auto const &input, match_params mp, capture_storage<N> &) noexcept
        {
            auto from = mp.from;
            bool res = (from < input.length() && A <= input[from] && input[from] <= B) ^mp.negated;
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

    template<std::size_t ID>
    struct backref : terminal
    {
        template<std::size_t N>
        static constexpr match_result match(auto const &input, match_params mp, capture_storage<N> &captures) noexcept
        {
            decltype(auto) str_to_match = std::get<ID>(captures).evaluate(input);
            std::size_t max_match = str_to_match.length();
            std::size_t offset = 0;
            while (offset < max_match)
            {
                if(input[mp.from + offset] != str_to_match[offset])
                    return {0, false};
                ++offset;
            }
            return {max_match, true};
        }
    };

    // Decorators for AST nodes
    template<typename Inner>
    struct negated
    {
        static constexpr std::size_t capture_count = Inner::capture_count;

        template<std::size_t N>
        static constexpr match_result match(auto const &input, match_params mp, capture_storage<N> &captures) noexcept
        {
            return Inner::template match<N>(input, {mp.from, mp.max_chars, !mp.negated}, captures);
        }
    };

    template<typename Inner>
    struct atomic
    {
        static constexpr std::size_t capture_count = Inner::capture_count;

        template<std::size_t N>
        static constexpr match_result match(auto const &input, match_params mp, capture_storage<N> &captures) noexcept
        {
            return Inner::template match<N>(input, mp, captures);
        }
    };

    template<std::size_t ID, typename Inner>
    struct capturing
    {
        static constexpr std::size_t capture_count = 1 + Inner::capture_count;

        template<std::size_t N>
        static constexpr match_result match(auto const &input, match_params mp, capture_storage<N> &captures) noexcept
        {
            auto inner_match = Inner::template match<N>(input, mp, captures);
            std::get<ID>(captures) = capture<ID>{mp.from, inner_match.count};
            return inner_match;
        }
    };

    template<std::size_t ID, typename Inner>
    struct capturing<ID, atomic<Inner>>
    {
        static constexpr std::size_t capture_count = 1 + Inner::capture_count;

        template<std::size_t N>
        static constexpr match_result match(auto const &input, match_params mp, capture_storage<N> &captures) noexcept
        {
            auto &this_capture = std::get<ID>(captures);
            if(this_capture.count)
                return {0, false};
            auto inner_match = Inner::template match<N>(input, mp, captures);
            this_capture = capture<ID>{mp.from, inner_match.count};
            return inner_match;
        }
    };
}
#endif //CX_AST_H
