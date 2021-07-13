#ifndef CX_CAPTURE_INDEXER_H
#define CX_CAPTURE_INDEXER_H

#include "ast.h"

/**
 * File with metafunctions for indexing captures
 */
namespace cx
{
    /**
     * Metafunction that indexes capturing groups according to Preorder AST traversal.
     *
     * During expression parsing and initial AST generation, capturing groups are
     * assigned IDs according to Postorder traversal. This is due to the stack-like
     * behavior of the parsing algorithm and needs to be fixed by this metafunction
     * to comply with indexing standards.
     *
     * @tparam Offset   The ID of the outer-most capturing group
     * @tparam Wrapper  The AST node type contained inside the capture
     */
    template<auto Offset, typename Wrapper>
    struct preorder_indexing
    {
        using type = Wrapper;
    };

    template<auto Offset, typename Wrapper>
    using preorder_indexing_t = typename preorder_indexing<Offset, Wrapper>::type;

    template<auto Offset, template<typename ...> typename Wrapper, typename ... Inner>
    struct preorder_indexing<Offset, Wrapper<Inner ...>>
    {
        using type = Wrapper<preorder_indexing_t<Offset, Inner> ...>;
    };

    template<auto Offset, typename ... Rest>
    struct preorder_indexing<Offset, sequence<Rest ...>>
    {
        using type = sequence<preorder_indexing_t<Offset, Rest> ...>;
    };

    template<auto Offset, auto N, typename Inner>
    struct preorder_indexing<Offset, capturing<N, Inner>>
    {
        static constexpr auto capture_count = Inner::capture_count;
        using type = capturing<N + Offset - capture_count, preorder_indexing_t<Offset + 1, Inner>>;
    };
}
#endif //CX_CAPTURE_INDEXER_H
