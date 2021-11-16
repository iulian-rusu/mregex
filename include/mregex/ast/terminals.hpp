#ifndef MREGEX_TERMINALS_HPP
#define MREGEX_TERMINALS_HPP

#include <mregex/regex_context.hpp>
#include <mregex/ast/astfwd.hpp>
#include <mregex/ast/ast_traits.hpp>
#include <mregex/ast/match_result.hpp>

namespace meta::ast
{
    struct terminal
    {
        static constexpr std::size_t capture_count = 0;
    };

    struct epsilon : terminal
    {
        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter, Iter, Iter it, Context &, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            return cont(it);
        }
    };

    struct nothing : terminal
    {
        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter, Iter, Iter it, Context &, Continuation &&) noexcept
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
            return First::consume_one(current, ctx) || (Rest::consume_one(current, ctx) || ...);
        }
    };

    struct beginning : terminal
    {
        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &, Continuation &&cont) noexcept
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

    struct ending : terminal
    {
        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter, Iter end, Iter it, Context &, Continuation &&cont) noexcept
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
    struct literal : terminal
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
        static constexpr bool consume_one(Iter current, Context &) noexcept
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
        static constexpr bool consume_one(Iter current, Context &) noexcept
        {
            auto ch = *current;
            bool res = A <= ch && ch <= B;
            if constexpr (flags<Context>::ignore_case)
            {
                ch = to_lower(ch);
                res |= A <= ch && ch <= B;
                ch = to_upper(ch);
                res |= A <= ch && ch <= B;
            }
            return res;
        }
    };

    template<auto A>
    struct range<A, A> : literal<A> {};

    template<std::size_t ID>
    struct backref : terminal
    {
        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            auto const captured = std::get<ID>(ctx.captures);
            std::size_t const length_to_match = captured.length();
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

    template<typename Inner>
    struct negated : terminal
    {
        static_assert(is_trivially_matchable_v<Inner>);

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
            return !Inner::consume_one(current, ctx);
        }
    };
}
#endif //MREGEX_TERMINALS_HPP