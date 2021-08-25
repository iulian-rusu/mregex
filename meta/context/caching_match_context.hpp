#ifndef META_CACHING_MATCH_CONTEXT_HPP
#define META_CACHING_MATCH_CONTEXT_HPP

#include "match_cache.hpp"

namespace meta
{
    /**
     * Extra match context used to cache intermediate results
     * during regex matching.
     *
     * @tparam Iter The forward iterator type used to acces the input sequence
     * @tparam N    The maximum capacity of the fixed-size cache
     */
    template<std::forward_iterator Iter, std::size_t N, bool = true>
    struct caching_match_context
    {
        match_cache<Iter, N> cache{};

        constexpr void clear_cache() noexcept
        {
            cache.clear();
        }
    };

    /**
     * Empty base class used if the Abstract Syntax Tree does not contain atomic groups.
     */
    template<std::forward_iterator Iter, std::size_t N>
    struct caching_match_context<Iter, N, false>
    {
        constexpr void clear_cache() const noexcept {}
    };
}
#endif //META_CACHING_MATCH_CONTEXT_HPP
