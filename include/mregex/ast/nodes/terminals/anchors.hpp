#ifndef MREGEX_NODES_ANCHORS_HPP
#define MREGEX_NODES_ANCHORS_HPP

#include <mregex/ast/nodes/assertion.hpp>
#include <mregex/ast/predicates/anchor_predicates.hpp>
#include <mregex/ast/predicates/negate.hpp>

namespace meta::ast
{
    template<typename Predicate>
    struct anchor : assertion<Predicate>, terminal {};

    struct beginning_of_line : anchor<predicates::beginning_predicate<line_mode::multiline>> {};
    struct beginning_of_input : anchor<predicates::beginning_predicate<line_mode::singleline>> {};
    struct end_of_line : anchor<predicates::end_predicate<line_mode::multiline>> {};
    struct end_of_input : anchor<predicates::end_predicate<line_mode::singleline>> {};
    struct word_boundary : anchor<predicates::word_boundary_predicate> {};

    /**
     * The generic implementation only allows negating trivially matchable nodes.
     * Since word boundaries are not trivially matchable, this template specialization is required.
     */
    template<>
    struct negated<word_boundary> : anchor<predicates::negate<predicates::word_boundary_predicate>> {};
}
#endif //MREGEX_NODES_ANCHORS_HPP