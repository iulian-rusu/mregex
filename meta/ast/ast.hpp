#ifndef META_AST_HPP
#define META_AST_HPP

#include "capture_counter.hpp"
#include "ast_traits.hpp"
#include "star_strategy.hpp"
#include "../regex_capture.hpp"
#include "../regex_context.hpp"
#include "../continuation.hpp"
#include "../utility/char_traits.hpp"
#include "../utility/concepts.hpp"

namespace meta::ast
{
    template<typename First, typename... Rest>
    struct sequence
    {
        static constexpr std::size_t capture_count = capture_counter<First, Rest ...>::count;

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires (!is_trivially_matchable_v<First>)
        {
            auto continuation = [=, &ctx, &cont](Iter next_it) {
                return sequence<Rest ...>::match(begin, end, next_it, ctx, cont);
            };
            return First::match(begin, end, it, ctx, continuation);
        }

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires (is_trivially_matchable_v<First> && !are_trivially_matchable_v<Rest ...>)
        {
            if (it == end)
                return {it, false};

            if (First::consume_one(it, ctx))
                return sequence<Rest ...>::match(begin, end, ++it, ctx, cont);
            return {it, false};
        }

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires are_trivially_matchable_v<First, Rest ...>
        {
            std::size_t const remaining_length = std::distance(it, end);
            if (remaining_length < sequence_size)
                return {it, false};
            return expand_trivial_match(begin, end, it, ctx, cont, std::make_index_sequence<sequence_size>{});
        }

    private:
        static constexpr std::size_t sequence_size = 1 + sizeof... (Rest);

        template<std::forward_iterator Iter, typename Context, typename Continuation, std::size_t Index, std::size_t... Indices>
        static constexpr match_result<Iter> expand_trivial_match(
                Iter,
                Iter,
                Iter it,
                Context &ctx,
                Continuation &&cont,
                std::index_sequence<Index, Indices ...> &&
        ) noexcept
        {
            if (First::consume_one(it, ctx) && (Rest::consume_one(it + Indices, ctx) && ...))
                return cont(it + sequence_size);
            return {it, false};
        }
    };

    template<typename First>
    struct sequence<First> : First {};


    template<bool, typename First, typename... Rest>
    struct alternation_base
    {
        static constexpr std::size_t capture_count = capture_counter<First, Rest ...>::count;

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            if (auto first_match = First::match(begin, end, it, ctx, cont))
                return first_match;
            return alternation<Rest ...>::match(begin, end, it, ctx, cont);
        }
    };

    template<typename First, typename... Rest>
    struct alternation_base<true, First, Rest ...> : set<First, Rest ...> {};

    template<typename First, typename... Rest>
    struct alternation : alternation_base<are_trivially_matchable_v<First, Rest ...>, First, Rest ...> {};

    template<typename First>
    struct alternation<First> : First {};

    template<typename First, typename... Rest>
    struct disjunction
    {
        static constexpr std::size_t capture_count = max_capture_counter<First, Rest ...>::count;

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            auto first_match = First::match(begin, end, it, ctx, cont);
            if (first_match && first_match.end == end)
                return first_match;
            ctx.clear();
            return disjunction<Rest ...>::match(begin, end, it, ctx, cont);
        }
    };

    template<typename First>
    struct disjunction<First> : First {};

    template<typename Inner>
    struct star
    {
        static constexpr std::size_t capture_count = Inner::capture_count;

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            return star_strategy<Inner, flags<Context>::ungreedy>::match(begin, end, it, ctx, cont);
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

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires (A > 0u)
        {
            if (auto exact_match = exact_repetition<A, Inner>::match(begin, end, it, ctx, continuations<Iter>::empty))
                return consume_rest(begin, end, exact_match.end, ctx, cont);
            return {it, false};
        }

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires (A == 0u)
        {
            return consume_rest(begin, end, it, ctx, cont);
        }

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto consume_rest(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires (!is_trivially_matchable_v<Inner>)
        {
            std::size_t matched_so_far = 0;
            while (matched_so_far < R)
            {
                auto inner_match = Inner::match(begin, end, it, ctx, continuations<Iter>::empty);
                if (!inner_match)
                    break;
                it = inner_match.end;
                ++matched_so_far;
            }
            return cont(it);
        }

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto consume_rest(Iter, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires is_trivially_matchable_v<Inner>
        {
            std::size_t matched_so_far = 0;
            for (; it != end && matched_so_far < R; ++it)
            {
                if (!Inner::consume_one(it, ctx))
                    break;
                ++matched_so_far;
            }
            return cont(it);
        }
    };

    // The right end of the interval is infinity
    template<std::size_t N, typename Inner>
    struct repetition<symbol::quantifier_value<N>, symbol::quantifier_inf, Inner>
    {
        static constexpr std::size_t capture_count = Inner::capture_count;

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            if (auto exact_match = exact_repetition<N, Inner>::match(begin, end, it, ctx, continuations<Iter>::empty))
                return star<Inner>::match(begin, end, exact_match.end, ctx, cont);
            return {it, false};
        }
    };

    // Both ends of the interval are equal - exact repetition
    template<std::size_t N, typename Inner>
    struct repetition<symbol::quantifier_value<N>, symbol::quantifier_value<N>, Inner>
    {
        static constexpr std::size_t capture_count = Inner::capture_count;

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires (!is_trivially_matchable_v<Inner>)
        {
            auto continuation = [=, &ctx, &cont](Iter new_it) {
                return exact_repetition<N - 1, Inner>::match(begin, end, new_it, ctx, cont);
            };
            return Inner::match(begin, end, it, ctx, continuation);
        }

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        requires is_trivially_matchable_v<Inner>
        {
            std::size_t length = std::distance(it, end);
            if (length < N)
                return {it, false};

            std::size_t matched = 0;
            for (; matched < N; ++matched)
            {
                if (!Inner::consume_one(it++, ctx))
                    return {it, false};
            }
            return cont(it);
        }
    };

    // Both ends of the interval are 0 - empty repetition
    template<typename Inner>
    struct repetition<symbol::quantifier_value<0u>, symbol::quantifier_value<0u>, Inner>
    {
        static constexpr std::size_t capture_count = Inner::capture_count;

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter, Iter, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            return cont(it);
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
        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            return cont(it);
        }
    };

    struct nothing : terminal
    {
        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            return {it, false};
        }

        template<std::forward_iterator Iter, typename Context>
        static constexpr bool consume_one(Iter, Context &) noexcept
        {
            return false;
        }
    };

    template<typename First, typename... Rest>
    struct set : terminal
    {
        static_assert(is_trivially_matchable_v<First> && (is_trivially_matchable_v<Rest> && ...));

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            if (it == end)
                return {it, false};

            if (consume_one(it, ctx))
                return cont(++it);
            return {it, false};
        }

        template<std::forward_iterator Iter, typename Context>
        static constexpr bool consume_one(Iter current, Context &ctx) noexcept
        {
            return First::consume_one(current, ctx) || (Rest::consume_one(current, ctx) || ...);
        }
    };

    struct beginline : terminal
    {
        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            if constexpr (flags<Context>::multiline)
            {
                if (it != end && *it == '\n')
                    return cont(++it);
            }
            if (it == begin)
                return cont(it);
            return {it, false};
        }
    };

    struct endline : terminal
    {
        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            if constexpr (flags<Context>::multiline)
            {
                if (it != end && *it == '\n')
                    return cont(++it);
            }
            if (it == end)
                return cont(it);
            return {it, false};
        }
    };

    template<auto C>
    struct character : terminal
    {
        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            if (it == end)
                return {it, false};

            if (consume_one(it, ctx))
                return cont(++it);
            return {it, false};
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
        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            if (it == end)
                return {it, false};

            if (consume_one(it, ctx))
                return cont(++it);
            return {it, false};
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
        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            if (it == end)
                return {it, false};

            if (consume_one(it, ctx))
                return cont(++it);
            return {it, false};
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

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            if (it == end)
                return {it, false};

            if (consume_one(it, ctx))
                return cont(++it);
            return {it, false};
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
        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            auto const captured = std::get<ID>(ctx.captures);
            auto const length_to_match = captured.length();
            std::size_t const remaining_length = std::distance(it, end);
            if (length_to_match > remaining_length)
                return {it, false};

            for (auto c : captured)
            {
                auto subject = *it;
                if constexpr (flags<Context>::ignore_case)
                {
                    subject = to_lower(subject);
                    c = to_lower(c);
                }
                if (subject != c)
                    return {it, false};

                ++it;
            }
            return cont(it);
        }
    };

    template<std::size_t ID, typename Inner>
    struct capturing
    {
        static constexpr std::size_t capture_count = 1 + Inner::capture_count;

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            auto continuation = [&](Iter new_it) {
                std::get<ID>(ctx.captures) = regex_capture_view<ID, Iter>{it, new_it};
                return cont(new_it);
            };

            return  Inner::match(begin, end, it, ctx, continuation);
        }
    };

    template<typename Inner>
    struct negated
    {
        static_assert(is_trivially_matchable_v<Inner>);

        static constexpr std::size_t capture_count = Inner::capture_count;

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            if (it == end)
                return {it, false};

            if (consume_one(it, ctx))
                return cont(++it);
            return {it, false};
        }

        template<std::forward_iterator Iter, typename Context>
        static constexpr bool consume_one(Iter current, Context &ctx) noexcept
        {
            return !Inner::consume_one(current, ctx);
        }
    };
}
#endif //META_AST_HPP