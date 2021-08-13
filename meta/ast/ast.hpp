#ifndef META_AST_HPP
#define META_AST_HPP

#include <tuple>
#include "atomic_counter.hpp"
#include "capture_counter.hpp"
#include "match_params.hpp"
#include "match_result.hpp"
#include "../capture.hpp"
#include "../match_context.hpp"
#include "../utility/char_traits.hpp"
#include "../utility/concepts.hpp"

namespace meta::ast
{
    template<typename First, typename ... Rest>
    struct sequence
    {
        static constexpr std::size_t capture_count = capture_counter<First, Rest ...>::count;
        static constexpr std::size_t atomic_count = atomic_counter<First, Rest ...>::count;

        template<typename MatchContext>
        static constexpr match_result match(auto const &input, match_params mp, MatchContext &ctx) noexcept
        {
            std::size_t consume_limit = mp.consume_limit;
            do
            {
                auto first_match = First::match(input, {mp.from, consume_limit}, ctx);
                if (!first_match)
                    return {0, false};

                match_params updated_mp = mp.consume(first_match.consumed);
                if (auto rest_matched = sequence<Rest ...>::match(input, updated_mp, ctx))
                    return first_match + rest_matched;

                if (first_match.consumed == 0 || consume_limit == 0)
                    return {0, false};

                consume_limit = first_match.consumed - 1;
            } while (true);
        }
    };

    template<typename First>
    struct sequence<First>
    {
        static constexpr std::size_t capture_count = First::capture_count;
        static constexpr std::size_t atomic_count = First::atomic_count;

        template<typename MatchContext>
        static constexpr match_result match(auto const &input, match_params mp, MatchContext &ctx) noexcept
        {
            return First::match(input, mp, ctx);
        }
    };

    template<typename First, typename ... Rest>
    struct alternation
    {
        static constexpr std::size_t capture_count = capture_counter<First, Rest ...>::count;
        static constexpr std::size_t atomic_count = atomic_counter<First, Rest ...>::count;

        template<typename MatchContext>
        static constexpr auto match(auto const &input, match_params mp, MatchContext &ctx) noexcept
        -> std::enable_if_t<!flags<MatchContext>::greedy_alt, match_result>
        {
            auto first_match = First::match(input, mp, ctx);
            return first_match ?: alternation<Rest ...>::match(input, mp, ctx);
        }

        // SFINAE overload for greedy alternation, which makes it always verify all options
        // and pick the one that consumes the most characters. It is useful for patterns where an
        // alternation contains prefixes: (a|ab|abc)
        template<typename MatchContext>
        static constexpr auto match(auto const &input, match_params mp, MatchContext &ctx) noexcept
        -> std::enable_if_t<flags<MatchContext>::greedy_alt, match_result>
        {
            auto first_match = First::match(input, mp, ctx);
            if (first_match)
            {
                auto rest_match = alternation<Rest ...>::match(input, mp, ctx);
                if (rest_match && rest_match.consumed > first_match.consumed)
                    return rest_match;
                return first_match;
            }
            return alternation<Rest ...>::match(input, mp, ctx);
        }
    };

    template<typename First>
    struct alternation<First>
    {
        static constexpr std::size_t capture_count = First::capture_count;
        static constexpr std::size_t atomic_count = First::atomic_count;

        template<typename MatchContext>
        static constexpr match_result match(auto const &input, match_params mp, MatchContext &ctx) noexcept
        {
            return First::match(input, mp, ctx);
        }
    };

    /**
     * Disjunction is a special AST node used to implement meta::regex_union.
     * Unlike meta::alternation, meta::disjuction needs to consume all characters to match.
     */
    template<typename First, typename ... Rest>
    struct disjunction
    {
        static constexpr std::size_t capture_count = max_capture_counter<First, Rest ...>::count;
        static constexpr std::size_t atomic_count = max_atomic_counter<First, Rest ...>::count;

        template<typename MatchContext>
        static constexpr match_result match(auto const &input, match_params mp, MatchContext &ctx) noexcept
        {
            auto first_match = First::match(input, mp, ctx);
            if (first_match && first_match.consumed == mp.consume_limit)
                return first_match;

            ctx.reset();
            return disjunction<Rest ...>::match(input, mp, ctx);
        }
    };

    template<typename First>
    struct disjunction<First>
    {
        static constexpr std::size_t capture_count = First::capture_count;
        static constexpr std::size_t atomic_count = First::atomic_count;

        template<typename MatchContext>
        static constexpr match_result match(auto const &input, match_params mp, MatchContext &ctx) noexcept
        {
            return First::match(input, mp, ctx);
        }
    };

    template<typename Inner>
    struct star
    {
        static constexpr std::size_t capture_count = Inner::capture_count;
        static constexpr std::size_t atomic_count = Inner::atomic_count;

        template<typename MatchContext>
        static constexpr match_result match(auto const &input, match_params mp, MatchContext &ctx) noexcept
        {
            match_result res{0, true};
            if (mp.consume_limit == 0)
                return res;

            match_params updated_mp = mp;
            std::size_t str_length = input.length();
            while (updated_mp.from < str_length)
            {
                auto inner_match = Inner::match(input, updated_mp, ctx);
                if (!inner_match || inner_match.consumed == 0 || inner_match.consumed > updated_mp.consume_limit)
                    break;
                res += inner_match;
                updated_mp = updated_mp.consume(inner_match.consumed);
            }
            return res;
        }
    };

    template<>
    struct star<wildcard>
    {
        static constexpr std::size_t capture_count = 0;
        static constexpr std::size_t atomic_count = 0;

        template<typename MatchContext>
        static constexpr match_result match(auto const &input, match_params mp, MatchContext &ctx) noexcept
        {
            if constexpr (flags<MatchContext>::dotall)
                return {mp.consume_limit, true};

            for(auto consumed = 0u; consumed <= mp.consume_limit; ++consumed)
            {
                auto subject = input[mp.from + consumed];
                if (subject == '\n' || subject == '\r')
                    return {consumed, true};
            }

            return {mp.consume_limit, true};
        }
    };

    template<std::size_t N, typename Inner>
    struct repeated
    {
        static constexpr std::size_t capture_count = Inner::capture_count;
        static constexpr std::size_t atomic_count = Inner::atomic_count;

        template<typename MatchContext>
        static constexpr match_result match(auto const &input, match_params mp, MatchContext &ctx) noexcept
        {
            if constexpr (N == 0)
                return {0, true};

            match_result res{0, false};
            match_params updated_mp = mp;
            std::size_t matched_so_far = 0;
            while (matched_so_far < N)
            {
                auto inner_match = Inner::match(input, updated_mp, ctx);
                if (!inner_match || inner_match.consumed > updated_mp.consume_limit)
                    break;
                res += inner_match;
                ++matched_so_far;
                updated_mp = updated_mp.consume(inner_match.consumed);
            }

            if (matched_so_far != N)
                return {0, false};

            return res;
        }
    };

    template<std::size_t N>
    struct repeated<N, wildcard>
    {
        static constexpr std::size_t capture_count = 0;
        static constexpr std::size_t atomic_count = 0;

        template<typename MatchContext>
        static constexpr match_result match(auto const &input, match_params mp, MatchContext &ctx) noexcept
        {
            if constexpr (N == 0)
                return {0, true};

            if constexpr (flags<MatchContext>::dotall)
            {
                if (N <= mp.consume_limit)
                    return {N, true};
                return {0, false};
            }

            for(auto consumed = 0u; consumed <= N; ++consumed)
            {
                auto subject = input[mp.from + consumed];
                if (subject == '\n' || subject == '\r')
                    return {0, false};
            }
            return {N, true};
        }
    };

    struct terminal
    {
        static constexpr std::size_t capture_count = 0;
        static constexpr std::size_t atomic_count = 0;
    };

    struct epsilon : terminal
    {
        template<typename MatchContext>
        static constexpr match_result match(auto const &, match_params, MatchContext &) noexcept
        {
            return {0, true};
        }
    };

    struct nothing : terminal
    {
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
                return {0, false};

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
                return {0, false};

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
                return {0, true};

            if (mp.consume_limit == 0 || mp.from >= input.length())
                return {0, false};

            auto subject = input[mp.from];
            bool res = C == subject;
            if constexpr (flags<MatchContext>::ignore_case)
                res |= toggle_case_v<C> == subject;

            return {res, res};
        }
    };

    struct whitespace : terminal
    {
        template<typename MatchContext>
        static constexpr match_result match(auto const &input, match_params mp, MatchContext &) noexcept
        {
            if constexpr (flags<MatchContext>::extended)
                return {0, true};

            if (mp.consume_limit == 0 || mp.from >= input.length())
                return {0, false};

            auto subject = input[mp.from];
            bool res = subject == ' ' || subject == '\t' ||
                       subject == '\n' || subject == '\r' ||
                       subject == '\f' || subject == '\x0B';
            return {res, res};
        }
    };

    struct wildcard : terminal
    {
        template<typename MatchContext>
        static constexpr match_result match(auto const &input, match_params mp, MatchContext &) noexcept
        {
            if (mp.consume_limit == 0)
                return {0, false};

            if (mp.from >= input.length())
                return {0, false};

            if constexpr (flags<MatchContext>::dotall)
                return{1, true};

            auto subject = input[mp.from];
            bool res = subject != '\n' && subject != '\r';
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
                return {0, false};

            auto subject = input[mp.from];
            bool res = A <= subject && subject <= B;
            if constexpr (flags<MatchContext>::ignore_case)
            {
                subject = to_lower(subject);
                res |= A <= subject && subject <= B;
                subject = to_upper(subject);
                res |= A <= subject && subject <= B;
            }
            return {res, res};
        }
    };

    template<std::size_t ID>
    struct backref : terminal
    {
        template<typename MatchContext>
        static constexpr match_result match(auto const &input, match_params mp, MatchContext &ctx) noexcept
        {
            decltype(auto) str_to_match = std::get<ID>(ctx.captures).evaluate(input);
            std::size_t length_to_match = str_to_match.length();
            if (mp.consume_limit < length_to_match)
                return {0, false};

            std::size_t offset = 0;
            std::size_t input_length = input.length();
            while (offset < length_to_match && mp.from + offset < input_length)
            {
                auto subject = input[mp.from + offset];
                auto to_match = str_to_match[offset];
                if constexpr (flags<MatchContext>::ignore_case)
                {
                    subject = to_lower(subject);
                    to_match = to_lower(to_match);
                }
                if (subject != to_match)
                    return {0, false};
                ++offset;
            }
            return {length_to_match, true};
        }
    };

    // Decorators for AST nodes
    template<std::size_t ID, typename Inner>
    struct atomic
    {
        static constexpr std::size_t capture_count = Inner::capture_count;
        static constexpr std::size_t atomic_count = 1 + Inner::atomic_count;

        template<typename MatchContext>
        static constexpr match_result match(auto const &input, match_params mp, MatchContext &ctx) noexcept
        {
            if (ctx.atomic_match_states[ID]) [[likely]]
                return {0, false};


            auto inner_match = Inner::match(input, mp, ctx);
            ctx.atomic_match_states[ID] = static_cast<bool>(inner_match);
            return inner_match;
        }
    };

    template<std::size_t ID, typename Inner>
    struct capturing
    {
        static constexpr std::size_t capture_count = 1 + Inner::capture_count;
        static constexpr std::size_t atomic_count = Inner::atomic_count;

        template<typename MatchContext>
        static constexpr match_result match(auto const &input, match_params mp, MatchContext &ctx) noexcept
        {
            auto inner_match = Inner::match(input, mp, ctx);
            std::get<ID>(ctx.captures) = capture<ID>{mp.from, inner_match.consumed};
            return inner_match;
        }
    };

    template<typename Inner>
    struct negated
    {
        static constexpr std::size_t capture_count = Inner::capture_count;
        static constexpr std::size_t atomic_count = Inner::atomic_count;

        /**
         * Only a subset of AST nodes support negated mode while matching.
         * This is checked using the requires clause and a custom type trait.
         */
        template<typename MatchContext>
        static constexpr match_result match(auto const &input, match_params mp, MatchContext &ctx) noexcept
        requires is_negatable_v<Inner>
        {
            return Inner::match(input, mp, ctx).template consume_if_not_matched<1>();
        }
    };
}
#endif //META_AST_HPP