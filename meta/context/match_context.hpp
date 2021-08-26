#ifndef META_MATCH_CONTEXT_HPP
#define META_MATCH_CONTEXT_HPP

#include "atomic_match_context.hpp"
#include "match_context_base.hpp"
#include "caching_match_context.hpp"
#include "../utility/tuple_helpers.hpp"

namespace meta
{
    template<std::forward_iterator Iter, typename AST, typename ... Flags>
    struct match_context_factory
    {
        static_assert((is_flag_v<Flags> && ...), "invalid flag");

        struct flag_states
        {
            static constexpr bool ignore_case = is_flag_enabled_v<flag::ignore_case, Flags ...>;
            static constexpr bool dotall = is_flag_enabled_v<flag::dotall, Flags ...>;
            static constexpr bool multiline = is_flag_enabled_v<flag::multiline, Flags ...>;
            static constexpr bool greedy_alt = is_flag_enabled_v<flag::greedy_alt, Flags ...>;
            static constexpr bool cache = is_flag_enabled_v<flag::cache, Flags ...>;
        };

        using base_context = match_context_base<AST>;
        using atomic_context = atomic_match_context<AST>;
        using caching_context = caching_match_context<Iter, 256, flag_states::cache>;

        struct match_context : base_context, atomic_context, caching_context
        {
            using flags = flag_states;
            using ast_type = AST;
            using iterator_type = Iter;

            constexpr void clear() noexcept
            {
                base_context::clear_captures();
                atomic_context::clear_atomic_state();
                caching_context::clear_cache();
            }
        };
    };

    template<std::forward_iterator Iter, typename AST, typename ... Flags>
    using create_match_context = typename match_context_factory<Iter, AST, Flags ...>::match_context;

    template<typename Context>
    using flags = typename Context::flags;
}
#endif //META_MATCH_CONTEXT_HPP