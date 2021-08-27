#ifndef META_PARSER_HPP
#define META_PARSER_HPP

#include "grammar/grammar.hpp"
#include "ast/ast_builder.hpp"
#include "ast/capture_indexer.hpp"
#include "utility/static_string.hpp"

namespace meta
{
    /**
     * Compile-time parser using the LL(1) algorithm.
     *
     * @tparam pattern  The input to be parsed
     */
    template<auto pattern>
    struct parser
    {
        // Helper metafunction used to obtain input symbols for the parser
        template<std::size_t I, bool = I < pattern.length()>
        struct character_at
        {
            using type = ast::character<pattern[I]>;
        };

        template<std::size_t I>
        struct character_at<I, false>
        {
            using type = symbol::epsilon;
        };

        template<std::size_t I>
        using character_at_t = typename character_at<I>::type;

        // Metafunction that models the transition of the parser automaton
        template<std::size_t, typename, typename, typename>
        struct transition;

        // Main metafunction used to parse the pattern
        template<std::size_t I, typename AST, typename Stack, bool = symbol::is_ast_update_v<top<Stack>>>
        struct parse
        {
            using next_stack = pop<Stack>;
            using current_char = character_at_t<I>;
            using current_rule = grammar::rule_t<top<Stack>, current_char>;

            using type = typename transition<I, current_rule, AST, next_stack>::type;
        };

        template<std::size_t I, typename AST, typename Stack>
        struct parse<I, AST, Stack, true>
        {
            using next_stack = pop<Stack>;
            using prev_char = character_at_t<I - 1>;
            using next_ast = typename ast::update_ast<top<Stack>, prev_char, AST>::type;

            using type = typename parse<I, next_ast, next_stack>::type;
        };

        // Base case - push the rule on the stack
        template<std::size_t I, typename Rule, typename AST, typename Stack>
        struct transition
        {
            using type = typename parse<I, AST, typename Stack::template push<Rule>>::type;
        };

        // Don't push anything
        template<std::size_t I, typename AST, typename Stack>
        struct transition<I, grammar::ignore, AST, Stack>
        {
            using type = typename parse<I, AST, Stack>::type;
        };

        // Advance to the next character
        template<std::size_t I, typename AST, typename Stack>
        struct transition<I, grammar::advance, AST, Stack>
        {
            using type = typename parse<I + 1, AST, Stack>::type;
        };

        // Advance and also push the rest of the rules on the stack
        template<std::size_t I, typename AST, typename... Rules, typename... Rest>
        struct transition<I, stack<grammar::advance, Rules ...>, AST, stack<Rest ...>>
        {
            using type = typename parse<I + 1, AST, stack<Rules ..., Rest ...>>::type;
        };

        // Reject the input pattern
        template<std::size_t I, typename AST, typename Stack>
        struct transition<I, grammar::reject, AST, Stack>
        {
            using type = pair<std::false_type, AST>;
        };

        // Final transition - accept the input pattern
        template<std::size_t I, typename AST, typename Stack>
        struct transition<I, grammar::accept, AST, Stack>
        {
            using type = pair<std::true_type, AST>;
        };

        using parse_result = typename parse<0, stack<>, stack<symbol::start>>::type;
        using ast_stack = typename parse_result::second;
        using ast_type = std::conditional_t<is_empty_v<ast_stack>, ast::epsilon, ast::preorder_indexing_t<0, top<ast_stack>>>;

        static constexpr bool accepted = typename parse_result::first{};
    };

    template<auto pattern>
    using ast_of = typename parser<pattern>::ast_type;
}
#endif //META_PARSER_HPP