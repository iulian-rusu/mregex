#ifndef META_ATOMIC_MATCH_CONTEXT_HPP
#define META_ATOMIC_MATCH_CONTEXT_HPP

#include <array>
#include "../ast/ast_traits.hpp"
#include "../ast/match_result.hpp"

namespace meta
{
    /**
     * Extra match context needed to evaluate atomic groups.
     * Keeps track of match states to prevent backtracking.
     *
     * @tparam AST    The Abstract Syntax Tree that contains the atomic groups
     */
    template<typename AST, bool = ast::has_atomic_group_v<AST>>
    struct atomic_match_context
    {
        static constexpr std::size_t atomic_count = AST::atomic_count;

        std::array<ast::match_result, atomic_count> atomic_match_cache{};
        std::array<bool, atomic_count> atomic_match_states{};

        constexpr void clear_states() noexcept
        {
            atomic_match_states.fill(false);
        }
    };

    /**
     * Empty base class used if the Abstract Syntax Tree does not contain atomic groups.
     */
    template<typename AST>
    struct atomic_match_context<AST, false>
    {
        constexpr void clear_states() const noexcept {}
    };
}
#endif //META_ATOMIC_MATCH_CONTEXT_HPP
