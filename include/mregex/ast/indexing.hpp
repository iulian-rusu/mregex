#ifndef MREGEX_AST_INDEXING_HPP
#define MREGEX_AST_INDEXING_HPP

#include <utility>
#include <mregex/ast/astfwd.hpp>
#include <mregex/ast/traits.hpp>
#include <mregex/ast/transform.hpp>

namespace meta::ast
{
    namespace detail
    {
        template<std::size_t PrefixOffset, std::size_t... CaptureCounts>
        struct capture_index_offsets;

        template<std::size_t PrefixOffset, std::size_t... CaptureCounts>
        using capture_index_offsets_t = typename capture_index_offsets<PrefixOffset, CaptureCounts ...>::type;

        template<std::size_t PrefixOffset>
        struct capture_index_offsets<PrefixOffset>
        {
            using type = std::index_sequence<>;
        };

        template<std::size_t PrefixOffset, std::size_t First, std::size_t... Rest>
        struct capture_index_offsets<PrefixOffset, First, Rest ...>
        {
            using rest_offsets = capture_index_offsets_t<PrefixOffset + First, Rest ...>;
            using type = decltype([]<std::size_t... RestOffsets>(std::index_sequence<RestOffsets ...>) {
                return std::index_sequence<PrefixOffset, RestOffsets ...>{};
            }(rest_offsets{}));
        };

        template<std::size_t Offset>
        struct capture_indexer
        {
            template<typename Node>
            struct index : transform<Node, index> {};

            template<typename Node>
            requires (capture_count<Node> == 0)
            struct index<Node>
            {
                using type = Node;
            };

            template<std::size_t ID, typename Name, typename Inner>
            struct index<capture<ID, Name, Inner>>
            {
                using indexed_inner_type = typename capture_indexer<Offset + 1>::template index_t<Inner>;
                using type = capture<Offset, Name, indexed_inner_type>;
            };

            template<typename... Nodes>
            struct index<sequence<Nodes ...>>
            {
                using inner_offsets = capture_index_offsets_t<Offset, capture_count<Nodes> ...>;
                using type = decltype([]<std::size_t... Offsets>(std::index_sequence<Offsets ...>) {
                    return sequence<typename capture_indexer<Offsets>::template index<Nodes>::type ...>{};
                }(inner_offsets{}));
            };

            template<typename Node>
            using index_t = typename index<Node>::type;
        };
    }

    /**
     * Metafunction that indexes capturing groups according to Preorder AST traversal.
     * During expression parsing and initial AST generation, capturing groups are
     * assigned IDs according to Postorder traversal. This is due to the stack-like
     * behavior of the parsing algorithm.
     *
     * @tparam Node     The AST node type being indexed
     */
    template<typename Node>
    using index_captures_t = typename detail::capture_indexer<1>::template index_t<Node>;
}
#endif //MREGEX_AST_INDEXING_HPP