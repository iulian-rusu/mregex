#ifndef CX_AST_H
#define CX_AST_H

#include <tuple>
#include "capture_counter.h"
#include "capture.h"
#include "match_params.h"
#include "../match_result.h"
#include "../regex_flags.h"
#include "../utility/char_helpers.h"

namespace cx
{
    template<typename First, typename ... Rest>
    struct sequence
    {
        static constexpr std::size_t capture_count = count_captures<First, Rest ...>::capture_count;

        template<typename MatchContext>
        static constexpr match_result match(auto const &input, match_params mp, MatchContext &ctx) noexcept
        {
            std::size_t consume_limit = mp.consume_limit;
            do
            {
                auto first_match = First::template match<MatchContext>(input, {mp.from, consume_limit, mp.negated}, ctx);
                if (!first_match)
                {
                    return {0, false};
                }
                match_params updated_mp = mp.consume(first_match.consumed);
                if (auto rest_matched = sequence<Rest ...>::template match<MatchContext>(input, updated_mp, ctx))
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

        template<typename MatchContext>
        static constexpr match_result match(auto const &input, match_params mp, MatchContext &ctx) noexcept
        {
            return First::template match<MatchContext>(input, mp, ctx);
        }
    };

    template<typename First, typename ... Rest>
    struct alternation
    {
        static constexpr std::size_t capture_count = count_captures<First, Rest ...>::capture_count;

        template<typename MatchContext>
        static constexpr auto match(auto const &input, match_params mp, MatchContext &ctx) noexcept
        -> std::enable_if_t<!flags<MatchContext>::greedy_alt, match_result>
        {
            auto first_match = First::template match<MatchContext>(input, {mp.from, mp.consume_limit, false}, ctx);

            if (first_match)
            {
                return first_match ^ mp.negated;
            }

            return alternation<Rest ...>::template match<MatchContext>(input, mp, ctx);
        }

        // SFINAE check for greedy_alt flag, which makes the alternation always verify all options
        // and pick the one that consumes the most characters. It is useful for patterns where an
        // alternation contains prefixes (a|ab|abc)
        template<typename MatchContext>
        static constexpr auto match(auto const &input, match_params mp, MatchContext &ctx) noexcept
        -> std::enable_if_t<flags<MatchContext>::greedy_alt, match_result>
        {
            auto first_match = First::template match<MatchContext>(input, {mp.from, mp.consume_limit, false}, ctx);

            if (first_match)
            {
                auto rest_match = alternation<Rest ...>::template match<MatchContext>(input, mp, ctx);
                if (rest_match && rest_match.consumed > first_match.consumed)
                {
                    first_match = rest_match;
                }
                return first_match ^ mp.negated;
            }

            return alternation<Rest ...>::template match<MatchContext>(input, mp, ctx);
        }
    };

    template<typename First>
    struct alternation<First>
    {
        static constexpr std::size_t capture_count = First::capture_count;

        template<typename MatchContext>
        static constexpr match_result match(auto const &input, match_params mp, MatchContext &ctx) noexcept
        {
            return First::template match<MatchContext>(input, mp, ctx);
        }
    };

    template<typename Inner>
    struct star
    {
        static constexpr std::size_t capture_count = Inner::capture_count;

        template<typename MatchContext>
        static constexpr match_result match(auto const &input, match_params mp, MatchContext &ctx) noexcept
        {
            if (mp.consume_limit == 0)
            {
                return {0, true};
            }

            match_result res{0, true};
            match_params updated_mp = mp;
            std::size_t str_length = input.length();

            while(updated_mp.from < str_length)
            {
                auto inner_match = Inner::template match<MatchContext>(input, updated_mp, ctx);
                if (!inner_match || inner_match.consumed > updated_mp.consume_limit)
                {
                    break;
                }
                res += inner_match;
                updated_mp = updated_mp.consume(inner_match.consumed);
            }
            return res;
        }
    };

    struct terminal
    {
        static constexpr std::size_t capture_count = 0;
    };

    struct epsilon : terminal
    {
        // epsilon node always matches everything and consumes no characters
        template<typename MatchContext>
        static constexpr match_result match(auto const &, match_params, MatchContext &) noexcept
        {
            return {0, true};
        }
    };

    struct null : terminal
    {
        // null node never matches anything and consumes no characters
        template<typename MatchContext>
        static constexpr match_result match(auto const &, match_params, MatchContext &) noexcept
        {
            return {0, false};
        }
    };

    struct beginning : terminal
    {
        template<typename MatchContext>
        static constexpr match_result match(auto const &input, match_params mp, MatchContext &) noexcept
        {
            auto res = mp.from == 0u;
            bool consume = false;
            if constexpr (flags<MatchContext>::multiline)
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
        template<typename MatchContext>
        static constexpr match_result match(auto const &input, match_params mp, MatchContext &) noexcept
        {
            auto res = mp.from == input.length();
            bool consume = false;
            if constexpr (flags<MatchContext>::multiline)
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
        template<typename MatchContext>
        static constexpr match_result match(auto const &input, match_params mp, MatchContext &ctx) noexcept
        {
            if constexpr (flags<MatchContext>::extended && is_whitespace_v<C>)
            {
                return {0, true};
            }

            if (mp.consume_limit == 0 || mp.from >= input.length())
            {
                return {mp.negated, mp.negated};
            }

            auto subject = input[mp.from];
            bool res = C == subject;
            if constexpr (flags<MatchContext>::ignore_case)
            {
                res |= toggle_case_v<C> == subject;
            }
            res ^= mp.negated;
            return {res, res};
        }
    };

    struct whitespace : terminal
    {
        template<typename MatchContext>
        static constexpr match_result match(auto const &input, match_params mp, MatchContext &) noexcept
        {
            if constexpr (flags<MatchContext>::extended)
            {
                return {0, true};
            }

            if (mp.consume_limit == 0 || mp.from >= input.length())
            {
                return {mp.negated, mp.negated};
            }

            auto subject = input[mp.from];
            bool res = subject == ' ' || subject == '\t' ||
                       subject == '\n' || subject == '\r' ||
                       subject == '\f' || subject == '\x0B';
            res ^= mp.negated;
            return {res, res};
        }
    };

    struct wildcard : terminal
    {
        template<typename MatchContext>
        static constexpr match_result match(auto const &input, match_params mp, MatchContext &) noexcept
        {
            if (mp.consume_limit == 0)
            {
                return {mp.negated, mp.negated};
            }

            if (mp.from >= input.length())
            {
                return {0, false};
            }

            if constexpr (flags<MatchContext>::dotall)
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

        template<typename MatchContext>
        static constexpr match_result match(auto const &input, match_params mp, MatchContext &) noexcept
        {
            if (mp.consume_limit == 0 || mp.from >= input.length())
            {
                return {mp.negated, mp.negated};
            }

            auto subject = input[mp.from];
            bool res = A <= subject && subject <= B;
            if constexpr (flags<MatchContext>::ignore_case)
            {
                subject = to_lower(subject);
                res |= A <= subject && subject <= B;
                subject = to_upper(subject);
                res |= A <= subject && subject <= B;
            }
            res ^= mp.negated;
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

    using alpha = range<'A', 'z'>;

    using word = alternation<alpha, digit, character<'_'>>;

    using hexa = alternation<digit, range<'a', 'f'>, range<'A', 'F'>>;

    template<std::size_t ID>
    struct backref : terminal
    {
        template<typename MatchContext>
        static constexpr match_result match(auto const &input, match_params mp, MatchContext &ctx) noexcept
        {
            decltype(auto) str_to_match = std::get<ID>(ctx.captures).evaluate(input);
            std::size_t max_match = str_to_match.length();
            std::size_t offset = 0;
            std::size_t str_lenght = input.length();

            while (offset < max_match && mp.from + offset < str_lenght)
            {
                auto subject = input[mp.from + offset];
                auto to_match = str_to_match[offset];
                if constexpr (flags<MatchContext>::ignore_case)
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

        template<typename MatchContext>
        static constexpr match_result match(auto const &input, match_params mp, MatchContext &ctx) noexcept
        {
            return Inner::template match<MatchContext>(input, {mp.from, mp.consume_limit, !mp.negated}, ctx);
        }
    };

    template<typename Inner>
    struct atomic
    {
        static constexpr std::size_t capture_count = Inner::capture_count;

        template<typename MatchContext>
        static constexpr match_result match(auto const &input, match_params mp, MatchContext &ctx) noexcept
        {
            return Inner::template match<MatchContext>(input, mp, ctx);
        }
    };

    template<std::size_t ID, typename Inner>
    struct capturing
    {
        static constexpr std::size_t capture_count = 1 + Inner::capture_count;

        template<typename MatchContext>
        static constexpr match_result match(auto const &input, match_params mp, MatchContext &ctx) noexcept
        {
            auto inner_match = Inner::template match<MatchContext>(input, mp, ctx);
            std::get<ID>(ctx.captures) = capture<ID>{mp.from, inner_match.consumed};
            return inner_match;
        }
    };

    template<std::size_t ID, typename Inner>
    struct capturing<ID, atomic<Inner>>
    {
        static constexpr std::size_t capture_count = 1 + Inner::capture_count;

        template<typename MatchContext>
        static constexpr match_result match(auto const &input, match_params mp, MatchContext &ctx) noexcept
        {
            auto &this_capture = std::get<ID>(ctx.captures);
            if (this_capture.consumed)
            {
                return {0, false};
            }
            auto inner_match = Inner::template match<MatchContext>(input, mp, ctx);
            this_capture = capture<ID>{mp.from, inner_match.consumed};
            return inner_match;
        }
    };
}
#endif //CX_AST_H
