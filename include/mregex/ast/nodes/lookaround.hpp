#ifndef MREGEX_NODES_LOOKAROUND_HPP
#define MREGEX_NODES_LOOKAROUND_HPP

#include <mregex/ast/nodes/assertion.hpp>
#include <mregex/ast/predicates/lookaround_predicate.hpp>
#include <mregex/ast/astfwd.hpp>

namespace meta::ast
{
    template<assertion_mode Mode, lookaround_direction Direction, typename Inner>
    struct lookaround : assertion<predicates::lookaround_predicate<Mode, Direction, Inner>>
    {
        static constexpr std::size_t capture_count = Inner::capture_count;
    };
}
#endif //MREGEX_NODES_LOOKAROUND_HPP