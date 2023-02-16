#ifndef MREGEX_PREDICATES_LOOKAROUND_PREDICATE_HPP
#define MREGEX_PREDICATES_LOOKAROUND_PREDICATE_HPP

#include <mregex/ast/predicates/negate.hpp>
#include <mregex/ast/inversion.hpp>
#include <mregex/ast/traits.hpp>
#include <mregex/utility/continuations.hpp>

namespace meta::ast::predicates
{
    template<typename Inner>
    struct lookahead_predicate
    {
        template<std::forward_iterator Iter, typename Context>
        static constexpr bool is_match(Iter begin, Iter end, Iter it, Context &ctx) noexcept
        {
            auto inner_match = Inner::match(begin, end, it, ctx, continuations<Iter>::success);
            return inner_match.matched;
        }

        template<std::forward_iterator Iter, typename Context>
        static constexpr bool is_match(Iter, Iter end, Iter it, Context &ctx) noexcept
        requires is_trivially_matchable<Inner>
        {
            return it != end && Inner::match_one(it, ctx);
        }
    };

    template<typename Inner>
    struct lookbehind_predicate
    {
        template<std::bidirectional_iterator Iter, typename Context>
        static constexpr bool is_match(Iter begin, Iter end, Iter it, Context &ctx) noexcept
        {
            // For non-trivial nodes, the AST is inverted to match the regex backwards
            using ast_type = invert_t<Inner>;
            using iterator_type = std::reverse_iterator<Iter>;

            auto rbegin = std::make_reverse_iterator(end); // Reversed end becomes new begin
            auto rend = std::make_reverse_iterator(begin); // Reversed begin becomes new end
            auto rit = std::make_reverse_iterator(it);
            auto result = ast_type::match(rbegin, rend, rit, ctx, continuations<iterator_type>::success);
            return result.matched;
        }

        template<std::bidirectional_iterator Iter, typename Context>
        static constexpr bool is_match(Iter begin, Iter, Iter it, Context &ctx) noexcept
        requires is_trivially_matchable<Inner>
        {
            // For trivially matchable nodes, a single step backwards is enough
            return it != begin && Inner::match_one(std::prev(it), ctx);
        }
    };

    template<assertion_mode Mode, lookaround_direction Direction,  typename Inner>
    struct lookaround_predicate;

    template<typename Inner>
    struct lookaround_predicate<assertion_mode::positive, lookaround_direction::ahead, Inner> : lookahead_predicate<Inner> {};

    template<typename Inner>
    struct lookaround_predicate<assertion_mode::negative, lookaround_direction::ahead, Inner> : negate<lookahead_predicate<Inner>> {};

    template<typename Inner>
    struct lookaround_predicate<assertion_mode::positive, lookaround_direction::behind, Inner> : lookbehind_predicate<Inner> {};

    template<typename Inner>
    struct lookaround_predicate<assertion_mode::negative, lookaround_direction::behind, Inner> : negate<lookbehind_predicate<Inner>> {};
}
#endif //MREGEX_PREDICATES_LOOKAROUND_PREDICATE_HPP