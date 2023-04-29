#ifndef MREGEX_NODES_ANCHORS_HPP
#define MREGEX_NODES_ANCHORS_HPP

#include <mregex/ast/nodes/assertion.hpp>
#include <mregex/ast/predicates/anchor_predicates.hpp>
#include <mregex/ast/astfwd.hpp>

namespace meta::ast
{
    struct beginning_of_line : assertion<predicates::beginning_predicate<line_mode::multiline>> {};
    struct beginning_of_input : assertion<predicates::beginning_predicate<line_mode::singleline>> {};
    struct end_of_line : assertion<predicates::end_predicate<line_mode::multiline>> {};
    struct end_of_input : assertion<predicates::end_predicate<line_mode::singleline>> {};
    struct word_boundary : assertion<predicates::word_boundary_predicate> {};
}
#endif //MREGEX_NODES_ANCHORS_HPP