#ifndef MREGEX_NODES_NEGATED_HPP
#define MREGEX_NODES_NEGATED_HPP

#include <mregex/ast/nodes/assertion.hpp>
#include <mregex/ast/nodes/trivially_matchable.hpp>
#include <mregex/ast/predicates/negate.hpp>
#include <mregex/ast/traits.hpp>

namespace meta::ast
{
    template<typename Inner>
    struct negated : trivially_matchable<negated<Inner>>
    {
        static_assert(is_trivially_matchable<Inner>, "to negate a generic AST node, it must be trivially matchable");

        template<std::forward_iterator Iter, typename Context>
        static constexpr bool match_one(Iter current, Context &ctx) noexcept
        {
            return !Inner::match_one(current, ctx);
        }
    };

    template<typename Inner>
    requires is_assertion<Inner>
    struct negated<Inner> : assertion<predicates::negate<typename Inner::predicate_type>> {};
}
#endif //MREGEX_NODES_NEGATED_HPP