#ifndef CX_AST_H
#define CX_AST_H

#include "capture_counter.h"
#include "match_params.h"
#include "../regex_result.h"
#include "../match_result.h"
#include "../match_context.h"
#include "../utility/char_helpers.h"

namespace cx
{
    template<typename First, typename ... Rest>
    struct sequence
    {
        static constexpr std::size_t capture_count = count_captures<First, Rest ...>::capture_count;

        template<typename Regex>
        static constexpr match_result match(auto const &input, match_params mp, match_context<Regex> &ctx) noexcept
        {
            std::size_t consume_limit = mp.consume_limit;
            do
            {
                auto first_match = First::template match<Regex>(input, {mp.from, consume_limit, mp.negated}, ctx);
                if (!first_match)
                {
                    return {0, false};
                }
                match_params updated_mp = mp.consume(first_match.consumed);
                if (auto rest_matched = sequence<Rest ...>::template match<Regex>(input, updated_mp, ctx))
                {
                    return first_match + rest_matched;
                }
                if (first_match.consumed == 0 || consume_limit == 0)
                {
                    return {0, false};
                }
                consume_limit = first_match.consumed - 1;
            } while (true);
        }
    };

    template<typename First>
    struct sequence<First>
    {
        static constexpr std::size_t capture_count = First::capture_count;

        template<typename Regex>
        static constexpr match_result match(auto const &input, match_params mp, match_context<Regex> &ctx) noexcept
        {
            return First::template match<Regex>(input, mp, ctx);
        }
    };

    template<typename First, typename ... Rest>
    struct alternation
    {
        static constexpr std::size_t capture_count = count_captures<First, Rest ...>::capture_count;

        template<typename Regex>
        static constexpr auto match(auto const &input, match_params mp, match_context<Regex> &ctx) noexcept
        -> std::enable_if_t<!flags<Regex>::greedy_alt, match_result>
        {
            auto first_match = First::template match<Regex>(input, {mp.from, mp.consume_limit, false}, ctx);

            if (first_match)
            {
                return first_match ^ mp.negated;
            }

            return alternation<Rest ...>::template match<Regex>(input, mp, ctx);
        }

        // SFINAE check for greedy_alt flag, which makes the alternation always verify all options
        // and pick the one that consumes the most characters. It is useful for patterns where an
        // alternation contains prefixes (a|ab|abc)
        template<typename Regex>
        static constexpr auto match(auto const &input, match_params mp, match_context<Regex> &ctx) noexcept
        -> std::enable_if_t<flags<Regex>::greedy_alt, match_result>
        {
            auto first_match = First::template match<Regex>(input, {mp.from, mp.consume_limit, false}, ctx);

            if (first_match)
            {
                auto rest_match = alternation<Rest ...>::template match<Regex>(input, mp, ctx);
                if (rest_match)
                {
                    first_match = rest_match;
                }
                return first_match ^ mp.negated;
            }

            return alternation<Rest ...>::template match<Regex>(input, mp, ctx);
        }
    };

    template<typename First>
    struct alternation<First>
    {
        static constexpr std::size_t capture_count = First::capture_count;

        template<typename Regex>
        static constexpr match_result match(auto const &input, match_params mp, match_context<Regex> &ctx) noexcept
        {
            return First::template match<Regex>(input, mp, ctx);
        }
    };

    template<typename Inner>
    struct star
    {
        static constexpr std::size_t capture_count = Inner::capture_count;

        template<typename Regex>
        static constexpr match_result match(auto const &input, match_params mp, match_context<Regex> &ctx) noexcept
        {
            if (mp.consume_limit == 0)
            {
                return {0, true};
            }

            if (auto inner_match = Inner::template match<Regex>(input, mp, ctx))
            {
                if (inner_match.consumed > mp.consume_limit)
                    return {0, true};
                match_params updated_mp = mp.consume(inner_match.consumed);
                return inner_match + match<Regex>(input, updated_mp, ctx);
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
        template<typename Regex>
        static constexpr match_result match(auto const &, match_params, match_context<Regex> &) noexcept
        {
            return {0, true};
        }
    };

    struct null : terminal
    {
        // null node never matches anything and consumes no characters
        template<typename Regex>
        static constexpr match_result match(auto const &, match_params, match_context<Regex> &) noexcept
        {
            return {0, false};
        }
    };

    struct beginning : terminal
    {
        template<typename Regex>
        static constexpr match_result match(auto const &input, match_params mp, match_context<Regex> &) noexcept
        {
            auto res = mp.from == 0u;
            bool consume = false;
            if constexpr (flags<Regex>::multiline)
            {
                consume = mp.from < input.length() && input[mp.from] == '\n';
                res |= consume;
            }

            if (mp.consume_limit == 0 && consume)
            {
                return {0, false};
            }

            return {consume, res};
        }
    };

    struct ending : terminal
    {
        template<typename Regex>
        static constexpr match_result match(auto const &input, match_params mp, match_context<Regex> &) noexcept
        {
            auto res = mp.from == input.length();
            bool consume = false;
            if constexpr (flags<Regex>::multiline)
            {
                consume = mp.from < input.length() && input[mp.from] == '\n';
                res |= consume;
            }

            if (mp.consume_limit == 0 && consume)
            {
                return {0, false};
            }

            return {consume, res};
        }
    };

    template<auto C>
    struct character : terminal
    {
        template<typename Regex>
        static constexpr match_result match(auto const &input, match_params mp, match_context<Regex> &ctx) noexcept
        {
            if constexpr (flags<Regex>::extended && is_whitespace_v<C>)
            {
                return {0, true};
            }

            if (mp.consume_limit == 0)
            {
                return {mp.negated, mp.negated};
            }

            if (mp.from < input.length())
            {
                auto subject = input[mp.from];
                bool res = C == subject;
                if constexpr (flags<Regex>::ignore_case)
                {
                    res |= toggle_case_v<C> == subject;
                }
                res ^= mp.negated;
                return {res, res};
            }
            return {mp.negated, mp.negated};
        }
    };

    struct whitespace : terminal
    {
        template<typename Regex>
        static constexpr match_result match(auto const &input, match_params mp, match_context<Regex> &) noexcept
        {
            if constexpr (flags<Regex>::extended)
            {
                return {0, true};
            }

            if (mp.consume_limit == 0)
            {
                return {mp.negated, mp.negated};
            }

            if (mp.from < input.length())
            {
                auto subject = input[mp.from];
                bool res = subject == ' ' || subject == '\t' ||
                           subject == '\n' || subject == '\r' ||
                           subject == '\f' || subject == '\x0B';
                res ^= mp.negated;
                return {res, res};
            }
            return {mp.negated, mp.negated};

        }
    };

    struct wildcard : terminal
    {
        template<typename Regex>
        static constexpr match_result match(auto const &input, match_params mp, match_context<Regex> &) noexcept
        {
            if (mp.consume_limit == 0)
            {
                return {mp.negated, mp.negated};
            }

            if (mp.from >= input.length())
            {
                return {0, false};
            }

            if constexpr (flags<Regex>::dotall)
            {
                return{1, true};
            }
            auto subject = input[mp.from];
            bool res = (subject != '\n' && subject != '\r') ^ mp.negated;
            return {res, res};
        }
    };

    template<auto A, auto B>
    struct range : terminal
    {
        static_assert(A < B, "invalid range parameters");

        template<typename Regex>
        static constexpr match_result match(auto const &input, match_params mp, match_context<Regex> &) noexcept
        {
            if (mp.consume_limit == 0)
            {
                return {mp.negated, mp.negated};
            }

            if (mp.from < input.length())
            {
                auto subject = input[mp.from];
                bool res = A <= subject && subject <= B;
                if constexpr (flags<Regex>::ignore_case)
                {
                    subject = to_lower(subject);
                    res |= A <= subject && subject <= B;
                    subject = to_upper(subject);
                    res |= A <= subject && subject <= B;
                }
                res ^= mp.negated;
                return {res, res};
            }
            return {mp.negated, mp.negated};
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
        template<typename Regex>
        static constexpr match_result match(auto const &input, match_params mp, match_context<Regex> &ctx) noexcept
        {
            decltype(auto) str_to_match = std::get<ID>(ctx.captures).evaluate(input);
            std::size_t max_match = str_to_match.length();
            std::size_t offset = 0;

            while (offset < max_match)
            {
                auto subject = input[mp.from + offset];
                auto to_match = str_to_match[offset];
                if constexpr (flags<Regex>::ignore_case)
                {
                    subject = to_lower(subject);
                    to_match = to_lower(to_match);
                }
                if (subject != to_match)
                {
                    return {0, false};
                }
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

        template<typename Regex>
        static constexpr match_result match(auto const &input, match_params mp, match_context<Regex> &ctx) noexcept
        {
            return Inner::template match<Regex>(input, {mp.from, mp.consume_limit, !mp.negated}, ctx);
        }
    };

    template<typename Inner>
    struct atomic
    {
        static constexpr std::size_t capture_count = Inner::capture_count;

        template<typename Regex>
        static constexpr match_result match(auto const &input, match_params mp, match_context<Regex> &ctx) noexcept
        {
            return Inner::template match<Regex>(input, mp, ctx);
        }
    };

    template<std::size_t ID, typename Inner>
    struct capturing
    {
        static constexpr std::size_t capture_count = 1 + Inner::capture_count;

        template<typename Regex>
        static constexpr match_result match(auto const &input, match_params mp, match_context<Regex> &ctx) noexcept
        {
            auto inner_match = Inner::template match<Regex>(input, mp, ctx);
            std::get<ID>(ctx.captures) = capture<ID>{mp.from, inner_match.consumed};
            return inner_match;
        }
    };

    template<std::size_t ID, typename Inner>
    struct capturing<ID, atomic<Inner>>
    {
        static constexpr std::size_t capture_count = 1 + Inner::capture_count;

        template<typename Regex>
        static constexpr match_result match(auto const &input, match_params mp, match_context<Regex> &ctx) noexcept
        {
            auto &this_capture = std::get<ID>(ctx.captures);
            if (this_capture.consumed)
            {
                return {0, false};
            }
            auto inner_match = Inner::template match<Regex>(input, mp, ctx);
            this_capture = capture<ID>{mp.from, inner_match.consumed};
            return inner_match;
        }
    };
}
#endif //CX_AST_H
