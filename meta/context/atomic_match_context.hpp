#ifndef META_ATOMIC_MATCH_CONTEXT_HPP
#define META_ATOMIC_MATCH_CONTEXT_HPP

#include <array>
#include "../ast/ast_traits.hpp"

namespace meta
{
    /**
     * Extra match context needed to evaluate atomic groups.
     * Contains an array that keeps track of atomic matches.
     *
     * @tparam Regex    The regex type that contains the atomic groups
     */
    template<typename Regex, bool = ast::has_atomic_group_v<typename Regex::ast_type>>
    struct atomic_match_context
    {
        static constexpr std::size_t atomic_count = Regex::atomic_count;
        std::array<bool, atomic_count> atomic_match_states{false};

        constexpr void clear_states() noexcept
        {
            for(auto i = 0u; i < atomic_count; ++i)
                atomic_match_states[i] = false;
        }
    };


    template<typename Regex>
    struct atomic_match_context<Regex, false>
    {
        // Empty object used when no atomic groups are contained in the regex
    };
}
#endif //META_ATOMIC_MATCH_CONTEXT_HPP
