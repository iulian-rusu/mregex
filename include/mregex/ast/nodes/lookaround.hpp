#ifndef MREGEX_NODES_LOOKAROUND_HPP
#define MREGEX_NODES_LOOKAROUND_HPP

#include <mregex/ast/nodes/assertion.hpp>
#include <mregex/ast/predicates/lookaround_predicate.hpp>
#include <mregex/ast/predicates/negate.hpp>
#include <mregex/ast/astfwd.hpp>

namespace meta::ast
{
    template<lookaround_direction Direction, typename Inner>
    struct lookaround: assertion<predicates::lookaround_predicate<Direction, Inner>> {};
}
#endif //MREGEX_NODES_LOOKAROUND_HPP