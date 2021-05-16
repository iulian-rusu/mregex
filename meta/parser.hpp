#ifndef META_PARSER_HPP
#define META_PARSER_HPP

#include "grammar/grammar.hpp"
#include "ast/ast_builder.hpp"
#include "ast/capture_indexer.hpp"
#include "utility/static_string.hpp"

namespace meta
{
    /**
     * Constexpr parser using the LL(1) algorithm.
     *
     * @tparam pattern  The input to be parsed
     */
    template<auto const pattern>
    struct parser
    {
        // Helper struct that contains either a symbol::character or symbol::epsilon based on the size of the index
        template<std::size_t I, bool = I < pattern.length()>
        struct character_at
        {
            using type = ast::character<pattern[I]>;
        };

        // Specialization for indices that are out of bound
        template<std::size_t I>
        struct character_at<I, false>
        {
            using type = symbol::epsilon;
        };

        template<std::size_t I>
        using character_at_t = typename character_at<I>::type;

        // Forward declare helper struct to resolve reference conflicts
        template<std::size_t, typename, typename, typename>
        struct next_step;

        // Main metafunction used to parse the pattern
        template<std::size_t I, typename AST, typename Stack, bool = symbol::is_ast_update_v<top<Stack>>>
        struct parse
        {
            using next_stack = pop<Stack>;
            using current_char = character_at_t<I>;
            using current_rule = grammar::rule_t<top<Stack>, current_char>;

            using type = typename next_step<I, current_rule, AST, next_stack>::type;
        };

        template<std::size_t I, typename AST, typename Stack>
        struct parse<I, AST, Stack, true>
        {
            using next_ast = typename ast::update_ast<top<Stack>, character_at_t<I - 1>, AST>::type;

            using type = typename parse<I, next_ast, pop<Stack>>::type;
        };

        // Metafunctions to decide the next step in the parsing algorithm
        template<std::size_t I, typename Rule, typename AST, typename Stack>
        struct next_step
        {
            using type = typename parse<I, AST, typename Stack::template push<Rule>>::type;
        };

        template<std::size_t I, typename AST, typename Stack>
        struct next_step<I, symbol::epsilon, AST, Stack>
        {
            using type = typename parse<I, AST, Stack>::type;
        };

        template<std::size_t I, typename AST, typename Stack>
        struct next_step<I, grammar::advance, AST, Stack>
        {
            using type = typename parse<I + 1, AST, Stack>::type;
        };

        template<std::size_t I, typename AST, typename Stack>
        struct next_step<I, grammar::reject, AST, Stack>
        {
            using type = pair<std::false_type, AST>;
        };

        template<std::size_t I, typename AST, typename Stack>
        struct next_step<I, grammar::accept, AST, Stack>
        {
            using type = pair<std::true_type, AST>;
        };

        using parse_result = typename parse<0, stack<>, stack<symbol::start>>::type;
        using ast_stack = typename parse_result::second;
        using ast_type = std::conditional_t<is_empty_v<ast_stack>, ast::epsilon, ast::preorder_indexing_t<0, top<ast_stack>>>;

        static constexpr bool accepted = typename parse_result::first{};
    };

    template<auto const pattern>
    using ast_of = typename parser<pattern>::ast_type;
}
#endif //META_PARSER_HPP