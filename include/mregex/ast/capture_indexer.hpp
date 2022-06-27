#ifndef MREGEX_CAPTURE_INDEXER_HPP
#define MREGEX_CAPTURE_INDEXER_HPP

#include <mregex/ast/astfwd.hpp>

namespace meta::ast
{
    /**
     * Metafunction that indexes capturing groups according to Preorder AST traversal.
     * During expression parsing and initial AST generation, capturing groups are
     * assigned IDs according to Postorder traversal. This is due to the stack-like
     * behavior of the parsing algorithm and needs to be fixed by this metafunction
     * to comply with indexing standards.
     *
     * @tparam Offset   The number of capturing groups from the current node to the AST root
     * @tparam Node     The AST node type being traversed
     */
    template<std::size_t Offset, typename Node>
    struct preorder_indexing
    {
        using type = Node;
    };

    template<std::size_t Offset, typename Node>
    using preorder_indexing_t = typename preorder_indexing<Offset, Node>::type;

    template<std::size_t Offset, template<typename...> typename Wrapper, typename... Inner>
    struct preorder_indexing<Offset, Wrapper<Inner ...>>
    {
        using type = Wrapper<preorder_indexing_t<Offset, Inner> ...>;
    };

    template<std::size_t Offset, typename A, typename B, typename Inner>
    struct preorder_indexing<Offset, repetition<A, B, Inner>>
    {
        using type = repetition<A, B, preorder_indexing_t<Offset, Inner>>;
    };

    template<std::size_t Offset, std::size_t I, typename Name, typename Inner>
    struct preorder_indexing<Offset, named_capture<I, Name, Inner>>
    {
        static constexpr std::size_t capture_count = Inner::capture_count;

        using type = named_capture<I + Offset - capture_count, Name, preorder_indexing_t<Offset + 1, Inner>>;
    };
}
#endif //MREGEX_CAPTURE_INDEXER_HPP