#ifndef CX_CAPTURE_INDEXER_H
#define CX_CAPTURE_INDEXER_H

#include "ast.h"

/**
 * File with metafunctions for correctly indexing capturing groups
 */
namespace cx
{
    /**
     * Metafunction that reverses the IDs of inner capturing groups
     * (3(2(1))) -> (1(2(3)))
     *
     * @tparam Offset   The ID of the outer-most capturing group
     * @tparam Wrapper  The AST node type contained inside the capture
     */
    template<auto Offset, typename Wrapper>
    struct index_captures
    {
        using type = Wrapper;
    };

    template<auto Offset, typename Wrapper>
    using index_captures_t = typename index_captures<Offset, Wrapper>::type;

    template<auto Offset, template<typename ...> typename Wrapper, typename ... Inner>
    struct index_captures<Offset, Wrapper<Inner ...>>
    {
        using type = Wrapper<index_captures_t<Offset, Inner> ...>;
    };

    template<auto Offset, typename ... Rest>
    struct index_captures<Offset, sequence < Rest ...>>
    {
        using type = sequence<index_captures_t<Offset, Rest> ...>;
    };

    template<auto Offset, auto N, typename Inner>
    struct index_captures<Offset, capturing<N, Inner>>
    {
        static constexpr auto capture_count = Inner::capture_count;
        using type = capturing<N + Offset - capture_count, index_captures_t<Offset + 1, Inner>>;
    };
}
#endif //CX_CAPTURE_INDEXER_H
