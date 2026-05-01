#ifndef MREGEX_NODES_ANCHORS_HPP
#define MREGEX_NODES_ANCHORS_HPP

#include <mregex/ast/nodes/assertion.hpp>
#include <mregex/ast/predicates/anchors.hpp>
#include <mregex/ast/astfwd.hpp>

namespace meta::ast
{
    struct beginning_of_input : assertion<predicates::beginning_of_input_predicate> {};
    struct end_of_input : assertion<predicates::end_of_input_predicate> {};

    struct beginning_of_line : assertion<predicates::beginning_of_line_predicate> {};
    struct end_of_line : assertion<predicates::end_of_line_predicate> {};

    struct word_boundary : assertion<predicates::word_boundary_predicate> {};
}
#endif //MREGEX_NODES_ANCHORS_HPP