#ifndef META_MATCH_CONTEXT_HPP
#define META_MATCH_CONTEXT_HPP

#include "utility/tuple_map.hpp"
#include "match_context_base.hpp"
#include "atomic_match_context.hpp"

namespace meta
{
    /**
     * Contains a meta::match_context that depends on a regex and matching flags.
     *
     * @tparam Regex    The type of regex used to match the input
     * @tparam Flags    A variable pack of meta::flag types
     */
    template<typename Regex, typename ... Flags>
    struct match_context_factory
    {
        static_assert((is_flag_v<Flags> && ... ), "invalid flag");

        using base_context = match_context_base<Regex, Flags ...>;
        using atomic_context =  atomic_match_context<Regex>;

        /**
         * Data structure associated with matching/searching.
         * Holds information like capture groups, regex flags and context for atomic groups.
         */
        struct match_context : base_context, atomic_context
        {
            constexpr void reset() noexcept
            {
                base_context::clear_captures();
                if constexpr (ast::has_atomic_group_v<typename Regex::ast_type>)
                    atomic_context::clear_states();
            }
        };
    };

    template<typename Regex, typename ... Flags>
    using create_match_context = typename match_context_factory<Regex, Flags ...>::match_context;

    template<typename Context>
    using flags = typename Context::flags;
}
#endif //META_MATCH_CONTEXT_HPP