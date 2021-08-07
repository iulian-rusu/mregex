#ifndef CX_MATCH_CONTEXT_ATOMIC_H
#define CX_MATCH_CONTEXT_ATOMIC_H

#include <array>
#include <algorithm>
#include "atomic.h"
#include "match_result.h"

namespace cx
{
    /**
     * Extra match context needed to evaluate atomic groups.
     * Contains an array that keeps track of atomic matches
     *
     * @tparam Regex    The Regex type that contains the atomic groups
     */
    template<typename Regex, bool = has_atomic_group_v<typename Regex::ast>>
    struct match_context_atomic
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
    struct match_context_atomic<Regex, false>
    {
        // Empty object used when no atomic groups are contained in the Regex
    };
}
#endif //CX_MATCH_CONTEXT_ATOMIC_H
