#ifndef MREGEX_PREDICATES_ANCHOR_PREDICATES_HPP
#define MREGEX_PREDICATES_ANCHOR_PREDICATES_HPP

#include <mregex/ast/nodes/terminals/range.hpp>
#include <mregex/ast/predicates/lookaround_predicate.hpp>
#include <mregex/ast/predicates/operators.hpp>
#include <mregex/ast/nodes/set.hpp>

namespace meta::ast::predicates
{
    struct beginning_of_input_predicate
    {
        template<std::forward_iterator Iter, typename Context>
        static constexpr bool is_match(Iter begin, Iter /*end*/, Iter current, Context &/*ctx*/) noexcept
        {
            return current == begin;
        }
    };

    struct end_of_input_predicate
    {
        template<std::forward_iterator Iter, typename Context>
        static constexpr bool is_match(Iter /*begin*/, Iter end, Iter current, Context &/*ctx*/) noexcept
        {
            return current == end;
        }
    };

    using beginning_of_line_predicate = logical_or<beginning_of_input_predicate, lookbehind_predicate<linebreak>>;
    using end_of_line_predicate = logical_or<end_of_input_predicate, lookahead_predicate<linebreak>>;

    using word_boundary_predicate = exclusive_or<lookbehind_predicate<word>, lookahead_predicate<word>>;
}
#endif //MREGEX_PREDICATES_ANCHOR_PREDICATES_HPP