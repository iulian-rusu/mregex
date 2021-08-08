#ifndef CX_MATCH_CONTEXT_H
#define CX_MATCH_CONTEXT_H

#include "utility/tuple_helpers.h"
#include "match_context_base.h"
#include "atomic_match_context.h"

namespace cx
{

    /**
     * Contains a cx::match_context that depends on a Regex and matching flags.
     *
     * @tparam Regex    The type of Regex used to match the input
     * @tparam Flags    A variable pack of cx::flag types
     */
    template<typename Regex, typename ... Flags>
    struct match_context_factory
    {
        static_assert((is_flag_v<Flags> && ... ), "invalid flag");

        using base_context = match_context_base<Regex, Flags ...>;
        using atomic_context =  atomic_match_context<Regex>;

        /**
         * Data structure associated with matching/searching.
         * Holds information like capture groups, Regex flags and context for atomic groups.
         */
        struct match_context : base_context, atomic_context
        {
            constexpr void reset() noexcept
            {
                base_context::clear_captures();
                if constexpr (has_atomic_group_v<typename Regex::ast>)
                    atomic_context::clear_states();
            }
        };
    };

    template<typename Regex, typename ... Flags>
    using create_match_context = typename match_context_factory<Regex, Flags ...>::match_context;

    template<typename Context>
    using flags = typename Context::flags;
}
#endif //CX_MATCH_CONTEXT_H