#ifndef MREGEX_PREDICATES_LOOKAROUND_HPP
#define MREGEX_PREDICATES_LOOKAROUND_HPP

#include <mregex/ast/inversion.hpp>
#include <mregex/ast/traits.hpp>
#include <mregex/utility/continuations.hpp>

namespace meta::ast::predicates
{
    template<lookaround_direction Direction, typename Inner>
    struct lookaround_predicate;

    template<typename Inner>
    struct lookaround_predicate<lookaround_direction::ahead, Inner>
    {
        template<std::forward_iterator Iter, typename Context>
        static constexpr bool is_match(Iter begin, Iter end, Iter current, Context &ctx) noexcept
        {
            auto inner_match = Inner::match(begin, end, current, ctx, continuations<Iter>::success);
            return inner_match.matched;
        }

        template<std::forward_iterator Iter, typename Context>
        static constexpr bool is_match(Iter /*begin*/, Iter end, Iter current, Context &ctx) noexcept
        requires is_trivially_matchable<Inner>
        {
            return current != end && Inner::match_one(*current, ctx);
        }
    };

    template<typename Inner>
    struct lookaround_predicate<lookaround_direction::behind, Inner>
    {
        template<std::bidirectional_iterator Iter, typename Context>
        static constexpr bool is_match(Iter begin, Iter end, Iter current, Context &ctx) noexcept
        {
            // For non-trivial nodes, the AST is inverted to match the regex backwards
            using ast_type = invert_t<Inner>;
            using iterator = std::reverse_iterator<Iter>;

            auto rbegin = std::make_reverse_iterator(end); // Reversed end becomes new begin
            auto rend = std::make_reverse_iterator(begin); // Reversed begin becomes new end
            auto rcurrent = std::make_reverse_iterator(current);
            auto result = ast_type::match(rbegin, rend, rcurrent, ctx, continuations<iterator>::success);
            return result.matched;
        }

        template<std::bidirectional_iterator Iter, typename Context>
        static constexpr bool is_match(Iter begin, Iter /*end*/, Iter current, Context &ctx) noexcept
        requires is_trivially_matchable<Inner>
        {
            // For trivially matchable nodes, a single step backwards is enough
            return current != begin && Inner::match_one(*std::prev(current), ctx);
        }
    };

    template<typename Inner>
    using lookahead_predicate = lookaround_predicate<lookaround_direction::ahead, Inner>;

    template<typename Inner>
    using lookbehind_predicate = lookaround_predicate<lookaround_direction::behind, Inner>;
}
#endif //MREGEX_PREDICATES_LOOKAROUND_HPP