#ifndef MREGEX_AST_INDEXING_HPP
#define MREGEX_AST_INDEXING_HPP

#include <mregex/ast/astfwd.hpp>
#include <mregex/ast/traversal.hpp>

namespace meta::ast
{
    namespace detail
    {
        template<std::size_t Offset>
        struct preorder_index_provider
        {
            template<typename Node>
            struct index : traverse<index, Node> {};

            template<std::size_t I, typename Name, typename Inner>
            struct index<capture<I, Name, Inner>>
            {
                static constexpr std::size_t capture_count = Inner::capture_count;

                using indexed_inner_type = typename preorder_index_provider<Offset + 1>::template type<Inner>;
                using type = capture<I + Offset - capture_count, Name, indexed_inner_type>;
            };

            template<typename Node>
            using type = typename index<Node>::type;
        };
    }

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
    struct preorder_index
    {
        using type = typename detail::preorder_index_provider<Offset>::template type<Node>;
    };

    template<std::size_t Offset, typename Node>
    using preorder_index_t = typename preorder_index<Offset, Node>::type;
}
#endif //MREGEX_AST_INDEXING_HPP