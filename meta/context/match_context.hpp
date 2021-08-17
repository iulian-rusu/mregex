#ifndef META_MATCH_CONTEXT_HPP
#define META_MATCH_CONTEXT_HPP

#include "match_context_base.hpp"
#include "atomic_match_context.hpp"
#include "../utility/tuple_map.hpp"

namespace meta
{
    /**
     * Contains a meta::match_context that depends on a regex and matching flags.
     *
     * @tparam AST      The Abstract Syntax Tree of the regex
     * @tparam Flags    A variable pack of meta::flag types
     */
    template<typename AST, typename ... Flags>
    struct match_context_factory
    {
        static_assert((is_flag_v<Flags> && ... ), "invalid flag");

        using base_context = match_context_base<AST, Flags ...>;
        using atomic_context = atomic_match_context<AST>;

        /**
         * Data structure associated with matching/searching.
         * Holds information like capture groups, regex flags and context for atomic groups.
         */
        struct match_context : base_context, atomic_context
        {
            constexpr void reset() noexcept
            {
                base_context::clear_captures();
                atomic_context::clear_states();
            }
        };
    };

    template<typename AST, typename ... Flags>
    using create_match_context = typename match_context_factory<AST, Flags ...>::match_context;

    template<typename Context>
    using flags = typename Context::flags;
}
#endif //META_MATCH_CONTEXT_HPP