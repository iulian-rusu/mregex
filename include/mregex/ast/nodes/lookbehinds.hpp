#ifndef MREGEX_NODES_LOOKBEHINDS_HPP
#define MREGEX_NODES_LOOKBEHINDS_HPP

#include <mregex/ast/astfwd.hpp>
#include <mregex/ast/ast_inversion.hpp>

namespace meta::ast
{
    template<typename Inner>
    struct reverse_matcher
    {
        template<std::bidirectional_iterator Iter, typename Context>
        static constexpr bool can_match(Iter begin, Iter end, Iter it, Context &ctx) noexcept
        requires (!is_trivially_matchable_v<Inner>)
        {
            // For non-trivial nodes, the AST is inverted to match the regex backwards
            using ast_type = invert_t<Inner>;
            using iterator_type = std::reverse_iterator<Iter>;

            auto rbegin = std::make_reverse_iterator(end); // Reversed end becomes new begin
            auto rend = std::make_reverse_iterator(begin); // Reversed begin becomes new end
            auto rit = std::make_reverse_iterator(it);
            auto match = ast_type::match(rbegin, rend, rit, ctx, continuations<iterator_type>::epsilon);
            return  match == true;
        }

        template<std::bidirectional_iterator Iter, typename Context>
        static constexpr bool can_match(Iter begin, Iter, Iter it, Context &ctx) noexcept
        requires is_trivially_matchable_v<Inner>
        {
            // For trivially matchable nodes, a single step backwards is enough
            return (it != begin && Inner::match_one(it - 1, ctx));
        }
    };

    template<typename Inner>
    struct positive_lookbehind
    {
        static constexpr std::size_t capture_count = Inner::capture_count;

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context &ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            static_assert(std::bidirectional_iterator<Iter>, "lookbehinds require bidirectional iterators");

            if (reverse_matcher<Inner>::can_match(begin, end, it, ctx))
                return cont(it);
            return {it, false};
        }
    };

    template<typename Inner>
    struct negative_lookbehind
    {
        static constexpr std::size_t capture_count = Inner::capture_count;

        template<std::forward_iterator Iter, typename Context, typename Continuation>
        static constexpr auto match(Iter begin, Iter end, Iter it, Context & ctx, Continuation &&cont) noexcept
        -> match_result<Iter>
        {
            static_assert(std::bidirectional_iterator<Iter>, "lookbehinds require bidirectional iterators");

            if (reverse_matcher<Inner>::can_match(begin, end, it, ctx))
                return {it, false};
            return cont(it);
        }
    };
}
#endif //MREGEX_NODES_LOOKBEHINDS_HPP