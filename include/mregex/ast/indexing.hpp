#ifndef MREGEX_AST_INDEXING_HPP
#define MREGEX_AST_INDEXING_HPP

#include <mregex/ast/astfwd.hpp>
#include <mregex/ast/traits.hpp>
#include <mregex/ast/transform.hpp>

namespace meta::ast
{
    namespace detail
    {
        template<std::size_t Offset>
        struct preorder_capture_indexer
        {
            template<typename Node>
            struct index : transform<Node, index> {};

            template<std::size_t ID, typename Name, typename Inner>
            struct index<capture<ID, Name, Inner>>
            {
                using indexed_inner_type = typename preorder_capture_indexer<Offset + 1>::template index_t<Inner>;
                using type = capture<ID + Offset - capture_count<Inner>, Name, indexed_inner_type>;
            };

            template<typename Node>
            using index_t = typename index<Node>::type;
        };
    }

    /**
     * Metafunction that indexes capturing groups according to Preorder AST traversal.
     * During expression parsing and initial AST generation, capturing groups are
     * assigned IDs according to Postorder traversal. This is due to the stack-like
     * behavior of the parsing algorithm and needs to be fixed by this metafunction
     * to comply with indexing standards.
     *
     * @tparam Node     The AST node type being traversed
     */
    template<typename Node>
    using index_captures_t = typename detail::preorder_capture_indexer<0>::template index_t<Node>;
}
#endif //MREGEX_AST_INDEXING_HPP