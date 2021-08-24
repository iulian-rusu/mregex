#ifndef META_ATOMIC_MATCH_CONTEXT_HPP
#define META_ATOMIC_MATCH_CONTEXT_HPP

#include "../ast/ast_traits.hpp"

namespace meta
{
    template<typename AST, bool = ast::has_atomic_group_v<AST>>
    struct atomic_match_context
    {
        bool atomic_match_state{};

        constexpr void clear_atomic_state() noexcept
        {
            atomic_match_state = false;
        }

        [[nodiscard]] constexpr bool pop_atomic_state() noexcept
        {
            bool res = atomic_match_state;
            atomic_match_state = false;
            return res;
        }
    };

    template<typename AST>
    struct atomic_match_context<AST, false>
    {
        constexpr void clear_atomic_state() const noexcept {}
    };
}
#endif //META_ATOMIC_MATCH_CONTEXT_HPP
