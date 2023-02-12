#ifndef MREGEX_NODES_ANCHORS_HPP
#define MREGEX_NODES_ANCHORS_HPP

#include <mregex/ast/nodes/terminals/range.hpp>
#include <mregex/ast/nodes/terminals/set.hpp>

namespace meta::ast
{
    template<bool MultilineSensitive>
    struct beginning_anchor : terminal
    {
        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            if (it == begin)
                return cont(it);
            if constexpr (MultilineSensitive && Context::flags::multiline)
            {
                if (linebreak::match_one(std::prev(it), ctx))
                    return cont(it);
            }
            return {it, false};
        }
    };

    struct beginning_of_line : beginning_anchor<true> {};
    struct beginning_of_input : beginning_anchor<false> {};

    template<bool MultilineSensitive>
    struct end_anchor : terminal
    {
        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            if (it == end)
                return cont(it);
            if constexpr (MultilineSensitive && Context::flags::multiline)
            {
                if (linebreak::match_one(it, ctx))
                    return cont(it);
            }
            return {it, false};
        }
    };

    struct end_of_line : end_anchor<true> {};
    struct end_of_input : end_anchor<false> {};

    struct word_boundary : terminal
    {
        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            static_assert(std::bidirectional_iterator<Iter>, "word boundaries require bidirectional iterators");

            bool behind = it != begin && word::match_one(std::prev(it), ctx);
            bool ahead = it != end && word::match_one(it, ctx);
            if (behind ^ ahead)
                return cont(it);
            return {it, false};
        }
    };

    /**
     * The generic implementation only allows negating trivially matchable nodes.
     * Since anchors are not trivially matchable, this template specialization is required.
     */
    template<>
    struct negated<word_boundary> : terminal
    {
        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            if (word_boundary::match(begin, end, it, ctx, continuations<Iter>::success))
                return {it, false};
            return cont(it);
        }
    };
}
#endif //MREGEX_NODES_ANCHORS_HPP