#ifndef META_MATCH_CONTEXT_HPP
#define META_MATCH_CONTEXT_HPP

#include "match_context_base.hpp"
#include "atomic_match_context.hpp"
#include "../utility/tuple_map.hpp"

namespace meta
{
    template<typename AST, typename ... Flags>
    struct match_context_factory
    {
        static_assert((is_flag_v<Flags> && ... ), "invalid flag");

        using base_context = match_context_base<AST, Flags ...>;
        using atomic_context = atomic_match_context<AST>;

        struct match_context : base_context, atomic_context
        {
            constexpr void clear() noexcept
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