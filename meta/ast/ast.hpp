#ifndef META_AST_HPP
#define META_AST_HPP

#include "capture_counter.hpp"
#include "match_bounds.hpp"
#include "match_result.hpp"
#include "../regex_capture.hpp"
#include "../context/match_context.hpp"
#include "../utility/char_traits.hpp"
#include "../utility/concepts.hpp"

namespace meta::ast
{
    template<typename First, typename ... Rest>
    struct sequence
    {
        static constexpr std::size_t capture_count = capture_counter<First, Rest ...>::count;

        template<std::forward_iterator Iter, typename Context>
        static constexpr match_result match(Iter begin, Iter end, match_bounds<Iter> mb, Context &ctx) noexcept
        requires (!is_trivially_matchable_v<First> && !has_atomic_group_v<First>)
        {
            std::size_t consume_limit = mb.consume_limit;
            while (true)
            {
                auto first_match = First::match(begin, end, {mb.from, consume_limit}, ctx);
                if (!first_match)
                    return {0, false};

                if (auto rest_match = sequence<Rest ...>::match(begin, end, mb.advance(first_match.consumed), ctx))
                    return first_match + rest_match;

                if (consume_limit == 0 || first_match.consumed == 0)
                    return {0, false};

                consume_limit = first_match.consumed - 1;
            }
        }

        template<std::forward_iterator Iter, typename Context>
        static constexpr match_result match(Iter begin, Iter end, match_bounds<Iter> mb, Context &ctx) noexcept
        requires (!is_trivially_matchable_v<First> && has_atomic_group_v<First>)
        {
            std::size_t consume_limit = mb.consume_limit;
            bool saved_match_state = ctx.pop_atomic_state();
            while (true)
            {
                auto first_match = First::match(begin, end, {mb.from, consume_limit}, ctx);
                if (!first_match)
                    break;

                if (auto rest_match = sequence<Rest ...>::match(begin, end, mb.advance(first_match.consumed), ctx))
                    return first_match + rest_match;

                if (ctx.atomic_match_state)
                    break;

                if (consume_limit == 0 || first_match.consumed == 0)
                    break;

                consume_limit = first_match.consumed - 1;
            }
            ctx.atomic_match_state = saved_match_state;
            return {0, false};
        }

        template<std::forward_iterator Iter, typename Context>
        static constexpr match_result match(Iter begin, Iter end, match_bounds<Iter> mb, Context &ctx) noexcept
        requires (is_trivially_matchable_v<First> && !are_trivially_matchable_v<Rest ...>)
        {
            if (mb.consume_limit == 0)
                return {0, false};

            if (First::consume_one(mb.from, ctx))
                if (auto rest_match = sequence<Rest ...>::match(begin, end, mb.advance(1), ctx))
                    return {rest_match.consumed + 1, true};
                
            return {0, false};
        }

        template<std::forward_iterator Iter, typename Context>
        static constexpr match_result match(Iter begin, Iter end, match_bounds<Iter> mb, Context &ctx) noexcept
        requires are_trivially_matchable_v<First, Rest ...>
        {
            if (mb.consume_limit < sequence_size)
                return {0, false};

            return expand_trivial_match(begin, end, mb, ctx, std::make_index_sequence<sequence_size>{});
        }
    private:
        static constexpr std::size_t sequence_size = 1 + sizeof ... (Rest);

        template<std::forward_iterator Iter, typename Context, std::size_t Index, std::size_t ... Indices>
        static constexpr match_result expand_trivial_match(
                Iter begin,
                Iter end,
                match_bounds<Iter> mb,
                Context &ctx,
                std::index_sequence<Index, Indices ...> &&
        ) noexcept
        {
            if (First::consume_one(mb.from, ctx) && (Rest::consume_one(mb.from + Indices, ctx) && ...))
                return {sequence_size, true};

            return {0, false};
        }
    };

    template<typename First>
    struct sequence<First> : First {};

    template<typename First, typename ... Rest>
    struct alternation
    {
        static constexpr std::size_t capture_count = capture_counter<First, Rest ...>::count;

        template<std::forward_iterator Iter, typename Context>
        static constexpr match_result match(Iter begin, Iter end, match_bounds<Iter> mb, Context &ctx) noexcept
        requires (!flags<Context>::greedy_alt)
        {
            auto first_match = First::match(begin, end, mb, ctx);
            return first_match ?: alternation<Rest ...>::match(begin, end, mb, ctx);
        }

        template<std::forward_iterator Iter, typename Context>
        static constexpr match_result match(Iter begin, Iter end, match_bounds<Iter> mb, Context &ctx) noexcept
        requires flags<Context>::greedy_alt
        {
            auto first_match = First::match(begin, end, mb, ctx);
            if (first_match)
            {
                auto rest_match = alternation<Rest ...>::match(begin, end, mb, ctx);
                if (rest_match && rest_match.consumed > first_match.consumed)
                    return rest_match;

                return first_match;
            }

            return alternation<Rest ...>::match(begin, end, mb, ctx);
        }
    };

    template<typename First>
    struct alternation<First> : First {};

    template<typename First, typename ... Rest>
    struct disjunction
    {
        static constexpr std::size_t capture_count = max_capture_counter<First, Rest ...>::count;

        template<std::forward_iterator Iter, typename Context>
        static constexpr match_result match(Iter begin, Iter end, match_bounds<Iter> mb, Context &ctx) noexcept
        {
            auto first_match = First::match(begin, end, mb, ctx);
            if (first_match && first_match.consumed == mb.consume_limit)
                return first_match;

            ctx.clear();
            return disjunction<Rest ...>::match(begin, end, mb, ctx);
        }
    };

    template<typename First>
    struct disjunction<First> : First {};

    template<typename Inner>
    struct star
    {
        static constexpr std::size_t capture_count = Inner::capture_count;

        template<std::forward_iterator Iter, typename Context>
        static constexpr match_result match(Iter begin, Iter end, match_bounds<Iter> mb, Context &ctx) noexcept
        requires (!is_trivially_matchable_v<Inner>)
        {
            match_result res{0, true};
            if (mb.consume_limit == 0)
                return res;

            if constexpr (flags<Context>::cache)
                if (auto cached = check_cache(mb, ctx))
                    return cached;

            match_bounds updated_mb = mb;
            while (updated_mb.consume_limit > 0)
            {
                auto inner_match = Inner::match(begin, end, updated_mb, ctx);
                if (!inner_match || inner_match.consumed == 0 || inner_match.consumed > updated_mb.consume_limit)
                    break;

                if constexpr (flags<Context>::cache)
                    ctx.cache.push({mb.from, res.consumed});
                res += inner_match;
                updated_mb = updated_mb.advance(inner_match.consumed);
            }
            return res;
        }

        template<std::forward_iterator Iter, typename Context>
        static constexpr match_result match(Iter begin, Iter end, match_bounds<Iter> mb, Context &ctx) noexcept
        requires is_trivially_matchable_v<Inner>
        {
            if constexpr (flags<Context>::cache)
                if (auto cached = check_cache(mb, ctx))
                    return cached;

            std::size_t consumed = 0;
            for (auto current = mb.from; current != mb.from + mb.consume_limit; ++current)
            {
                if (!Inner::consume_one(current, ctx))
                    return {consumed, true};

                ++consumed;
                if constexpr (flags<Context>::cache)
                    ctx.cache.push({mb.from, current});
            }

            return {mb.consume_limit, true};
        }
    private:
        template<std::forward_iterator Iter, typename Context>
        static constexpr match_result check_cache(match_bounds<Iter> mb, Context &ctx)
        requires flags<Context>::cache
        {
            while (!ctx.cache.empty())
            {
                auto cached = ctx.cache.top();
                if (cached.from != mb.from)
                    break;

                ctx.cache.pop();
                if (cached.consumed <= mb.consume_limit)
                    return {cached.consumed, true};
            }
            return {0, false};
        }
    };

    template<std::size_t A, typename Inner>
    using exact_repetition = repetition<symbol::quantifier_value<A>, symbol::quantifier_value<A>, Inner>;

    // Base case - both interval ends are finite
    template<std::size_t A, std::size_t B, typename Inner>
    struct repetition<symbol::quantifier_value<A>, symbol::quantifier_value<B>, Inner>
    {
        static_assert(A < B, "invalid repetition bounds");

        static constexpr std::size_t R = B - A;
        static constexpr std::size_t capture_count = Inner::capture_count;

        template<std::forward_iterator Iter, typename Context>
        static constexpr match_result match(Iter begin, Iter end, match_bounds<Iter> mb, Context &ctx) noexcept
        requires (A > 0u)
        {
            if (auto exactly_n_match = exact_repetition<A, Inner>::match(begin, end, mb, ctx))
            {
                match_bounds updated_mb = mb.advance(exactly_n_match.consumed);
                auto rest_match = consume_rest(begin, end, updated_mb, ctx);
                return exactly_n_match + rest_match;
            }
            return {0, false};
        }

        template<std::forward_iterator Iter, typename Context>
        static constexpr match_result match(Iter begin, Iter end, match_bounds<Iter> mb, Context &ctx) noexcept
        requires (A == 0u)
        {
            return consume_rest(begin, end, mb, ctx);
        }

        template<std::forward_iterator Iter, typename Context>
        static constexpr match_result consume_rest(Iter begin, Iter end, match_bounds<Iter> mb, Context &ctx) noexcept
        requires (!is_trivially_matchable_v<Inner>)
        {
            match_result res{0, true};
            match_bounds updated_mb = mb;
            std::size_t matched_so_far = 0;
            while (matched_so_far < R)
            {
                auto inner_match = Inner::match(begin, end, updated_mb, ctx);
                if (!inner_match || inner_match.consumed > updated_mb.consume_limit)
                    break;

                res += inner_match;
                ++matched_so_far;
                updated_mb = updated_mb.advance(inner_match.consumed);
            }

            return res;
        }

        template<std::forward_iterator Iter, typename Context>
        static constexpr match_result consume_rest(Iter begin, Iter end, match_bounds<Iter> mb, Context &ctx) noexcept
        requires is_trivially_matchable_v<Inner>
        {
            std::size_t const consume_limit = R <= mb.consume_limit ? R : mb.consume_limit;
            std::size_t consumed = 0;
            for (auto current = mb.from; current != mb.from + consume_limit; ++current)
            {
                if (!Inner::consume_one(current, ctx))
                    return {consumed, true};

                ++consumed;
            }

            return {consume_limit, true};
        }
    };

    // The right end of the interval is infinity
    template<std::size_t N, typename Inner>
    struct repetition<symbol::quantifier_value<N>, symbol::quantifier_inf, Inner>
    {
        static constexpr std::size_t capture_count = Inner::capture_count;

        template<std::forward_iterator Iter, typename Context>
        static constexpr match_result match(Iter begin, Iter end, match_bounds<Iter> mb, Context &ctx) noexcept
        {
            if (auto exactly_n_match = exact_repetition<N, Inner>::match(begin, end, mb, ctx))
            {
                match_bounds updated_mb = mb.advance(exactly_n_match.consumed);
                auto star_match = star<Inner>::match(begin, end, updated_mb, ctx);
                return exactly_n_match + star_match;
            }
            return {0, false};
        }
    };

    // Both ends of the interval are equal - exact repetition
    template<std::size_t N, typename Inner>
    struct repetition<symbol::quantifier_value<N>, symbol::quantifier_value<N>, Inner>
    {
        static constexpr std::size_t capture_count = Inner::capture_count;

        template<std::forward_iterator Iter, typename Context>
        static constexpr match_result match(Iter begin, Iter end, match_bounds<Iter> mb, Context &ctx) noexcept
        requires (!is_trivially_matchable_v<Inner>)
        {
            match_result res{0, false};
            match_bounds updated_mb = mb;
            std::size_t matched_so_far = 0;
            while (matched_so_far < N)
            {
                auto inner_match = Inner::match(begin, end, updated_mb, ctx);
                if (!inner_match || inner_match.consumed > updated_mb.consume_limit)
                    break;

                res += inner_match;
                ++matched_so_far;
                updated_mb = updated_mb.advance(inner_match.consumed);
            }

            if (matched_so_far != N)
                return {0, false};

            return res;
        }

        template<std::forward_iterator Iter, typename Context>
        static constexpr match_result match(Iter begin, Iter end, match_bounds<Iter> mb, Context &ctx) noexcept
        requires is_trivially_matchable_v<Inner>
        {
            if (N > mb.consume_limit)
                return {0, false};

            for (auto current = mb.from; current != mb.from + N; ++current)
                if (!Inner::consume_one(current, ctx))
                    return {0, false};

            return {N, true};
        }
    };

    // Both ends of the interval are 0 - empty repetition
    template<typename Inner>
    struct repetition<symbol::quantifier_value<0u>, symbol::quantifier_value<0u>, Inner>
    {
        static constexpr std::size_t capture_count = Inner::capture_count;

        template<std::forward_iterator Iter, typename Context>
        static constexpr match_result match(Iter begin, Iter end, match_bounds<Iter> mb, Context &ctx) noexcept
        {
            return {0, true};
        }
    };

    // Interval from 0 to infinity - equivalent to star operator
    template<typename Inner>
    struct repetition<symbol::quantifier_value<0u>, symbol::quantifier_inf, Inner> : star<Inner> {};

    struct terminal
    {
        static constexpr std::size_t capture_count = 0;
    };

    struct epsilon : terminal
    {
        template<std::forward_iterator Iter, typename Context>
        static constexpr match_result match(Iter begin, Iter end, match_bounds<Iter> mb, Context &ctx) noexcept
        {
            return {0, true};
        }
    };

    struct nothing : terminal
    {
        template<std::forward_iterator Iter, typename Context>
        static constexpr match_result match(Iter begin, Iter end, match_bounds<Iter> mb, Context &ctx) noexcept
        {
            return {0, false};
        }

        template<std::forward_iterator Iter, typename Context>
        static constexpr bool consume_one(Iter, Context &) noexcept
        {
            return false;
        }
    };

    template<typename First, typename ... Rest>
    struct set : terminal
    {
        static_assert(is_trivially_matchable_v<First> && (is_trivially_matchable_v<Rest> && ...));

        template<std::forward_iterator Iter, typename Context>
        static constexpr match_result match(Iter begin, Iter end, match_bounds<Iter> mb, Context &ctx) noexcept
        {
            if (mb.consume_limit == 0)
                return {0, false};

            bool const res = consume_one(mb.from, ctx);
            return {res, res};
        }

        template<std::forward_iterator Iter, typename Context>
        static constexpr bool consume_one(Iter current, Context &ctx) noexcept
        {
            return First::consume_one(current, ctx) || (Rest::consume_one(current, ctx) || ...);
        }
    };

    struct beginning : terminal
    {
        template<std::forward_iterator Iter, typename Context>
        static constexpr match_result match(Iter begin, Iter end, match_bounds<Iter> mb, Context &ctx) noexcept
        {
            bool res = mb.from == begin;
            bool consume = false;
            if constexpr (flags<Context>::multiline)
            {
                consume = mb.from != end && *mb.from == '\n';
                res |= consume;
            }

            if (mb.consume_limit == 0 && consume)
                return {0, false};

            return {consume, res};
        }
    };

    struct ending : terminal
    {
        template<std::forward_iterator Iter, typename Context>
        static constexpr match_result match(Iter begin, Iter end, match_bounds<Iter> mb, Context &ctx) noexcept
        {
            bool res = mb.from == end;
            bool consume = false;
            if constexpr (flags<Context>::multiline)
            {
                consume = mb.from != end && *mb.from == '\n';
                res |= consume;
            }

            if (mb.consume_limit == 0 && consume)
                return {0, false};

            return {consume, res};
        }
    };

    template<auto C>
    struct character : terminal
    {
        template<std::forward_iterator Iter, typename Context>
        static constexpr match_result match(Iter begin, Iter end, match_bounds<Iter> mb, Context &ctx) noexcept
        {
            if (mb.consume_limit == 0)
                return {0, false};

            bool const res = consume_one(mb.from, ctx);
            return {res, res};
        }

        template<std::forward_iterator Iter, typename Context>
        static constexpr bool consume_one(Iter current, Context &ctx) noexcept
        {
            bool res = C == *current;
            if constexpr (flags<Context>::ignore_case)
                res |= toggle_case_v<C> == *current;
            return res;
        }
    };

    struct whitespace : terminal
    {
        template<std::forward_iterator Iter, typename Context>
        static constexpr match_result match(Iter begin, Iter end, match_bounds<Iter> mb, Context &ctx) noexcept
        {
            if (mb.consume_limit == 0)
                return {0, false};

            bool const res = consume_one(mb.from, ctx);
            return {res, res};
        }

        template<std::forward_iterator Iter, typename Context>
        static constexpr bool consume_one(Iter current, Context &) noexcept
        {
            auto ch = *current;
            return ch == ' ' || ch == '\t' ||
                   ch == '\n' || ch == '\r' ||
                   ch == '\f' || ch == '\x0B';
        }
    };

    struct wildcard : terminal
    {
        template<std::forward_iterator Iter, typename Context>
        static constexpr match_result match(Iter begin, Iter end, match_bounds<Iter> mb, Context &ctx) noexcept
        {
            if (mb.consume_limit == 0)
                return {0, false};

            bool const res = consume_one(mb.from, ctx);
            return {res, res};
        }

        template<std::forward_iterator Iter, typename Context>
        static constexpr bool consume_one(Iter current, Context &) noexcept
        {
            if constexpr (flags<Context>::dotall)
                return true;

            auto ch = *current;
            return  ch != '\n' && ch != '\r';
        }
    };

    template<auto A, auto B>
    struct range : terminal
    {
        static_assert(A < B, "invalid range bounds");

        template<std::forward_iterator Iter, typename Context>
        static constexpr match_result match(Iter begin, Iter end, match_bounds<Iter> mb, Context &ctx) noexcept
        {
            if (mb.consume_limit == 0)
                return {0, false};

            bool const res = consume_one(mb.from, ctx);
            return {res, res};
        }

        template<std::forward_iterator Iter, typename Context>
        static constexpr bool consume_one(Iter current, Context &ctx) noexcept
        {
            auto ch = *current;
            bool res = A <= ch && ch <= B;
            if constexpr (flags<Context>::ignore_case)
            {
                auto tmp = to_lower(ch);
                res |= A <= tmp && tmp <= B;
                tmp = to_upper(ch);
                res |= A <= tmp && tmp <= B;
            }
            return res;
        }
    };

    template<auto A>
    struct range<A, A> : character<A> {};

    template<std::size_t ID>
    struct backref : terminal
    {
        template<std::forward_iterator Iter, typename Context>
        static constexpr match_result match(Iter begin, Iter end, match_bounds<Iter> mb, Context &ctx) noexcept
        {
            auto const captured = std::get<ID>(ctx.captures);
            for (auto c : captured)
            {
                if (mb.from == end)
                    return {0, false};

                auto subject = *mb.from;
                if constexpr (flags<Context>::ignore_case)
                {
                    subject = to_lower(subject);
                    c = to_lower(c);
                }
                if (subject != c)
                    return {0, false};

                ++mb.from;
            }
            return {captured.length(), true};
        }
    };

    template<typename Inner>
    struct atomic
    {
        static constexpr std::size_t capture_count = Inner::capture_count;

        template<std::forward_iterator Iter, typename Context>
        static constexpr match_result match(Iter begin, Iter end, match_bounds<Iter> mb, Context &ctx) noexcept
        {
            auto inner_match = Inner::match(begin, end, mb, ctx);
            ctx.atomic_match_state = static_cast<bool>(inner_match);
            return inner_match;
        }

        template<std::forward_iterator Iter, typename Context>
        static constexpr bool consume_one(Iter current, Context &ctx) noexcept
        requires is_trivially_matchable_v<Inner>
        {
            auto res = Inner::consume_one(current, ctx);
            ctx.atomic_match_state = res;
            return res;
        }
    };

    template<std::size_t ID, typename Inner>
    struct capturing
    {
        static constexpr std::size_t capture_count = 1 + Inner::capture_count;

        template<std::forward_iterator Iter, typename Context>
        static constexpr match_result match(Iter begin, Iter end, match_bounds<Iter> mb, Context &ctx) noexcept
        {
            auto inner_match = Inner::match(begin, end, mb, ctx);
            std::get<ID>(ctx.captures) = regex_capture_view<ID, Iter>{mb.from, mb.from + inner_match.consumed};
            return inner_match;
        }
    };

    template<typename Inner>
    struct negated
    {
        static_assert(is_trivially_matchable_v<Inner>);

        static constexpr std::size_t capture_count = Inner::capture_count;

        template<std::forward_iterator Iter, typename Context>
        static constexpr match_result match(Iter begin, Iter end, match_bounds<Iter> mb, Context &ctx) noexcept
        {
            if (mb.consume_limit == 0)
                return {0, false};

            bool const res = !Inner::consume_one(mb.from, ctx);
            return {res, res};
        }

        template<std::forward_iterator Iter, typename Context>
        static constexpr bool consume_one(Iter current, Context &ctx) noexcept
        {
            return !Inner::consume_one(current, ctx);
        }
    };
}
#endif //META_AST_HPP