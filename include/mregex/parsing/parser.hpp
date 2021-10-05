#ifndef MREGEX_PARSER_HPP
#define MREGEX_PARSER_HPP

#include <mregex/ast/ast_builder.hpp>
#include <mregex/ast/capture_indexer.hpp>
#include <mregex/grammar/grammar.hpp>
#include <mregex/parsing/lexer.hpp>
#include <mregex/parsing/parser_status.hpp>
#include <mregex/parsing/parser_result.hpp>

namespace meta
{
    /**
     * Compile-time parser using the LL(1) algorithm.
     *
     * @tparam pattern  The input string to be parsed
     */
    template<static_string pattern>
    struct parser
    {
        /**
         * Metafunction used to exctract tokens (characters or epsilon) from the input pattern.
         */
        template<std::size_t I>
        using token_t = typename lexer<pattern>::template token_t<I>;

        /**
         * Metafunction that models the transition of the parser automaton.
         */
        template<std::size_t, typename, typename, typename>
        struct transition;

        template<std::size_t I, typename Rule, typename AST, typename Stack>
        using transition_t = typename transition<I, Rule, AST, Stack>::type;

        /**
         * Main metafunction used to parse the pattern.
         */
        template<std::size_t I, typename AST, typename Stack, bool = symbol::is_ast_update_v<top<Stack>>>
        struct parse
        {
            using next_stack = pop<Stack>;
            using current_token = token_t<I>;
            using current_rule = grammar::rule_t<top<Stack>, current_token>;

            using type = transition_t<I, current_rule, AST, next_stack>;
        };

        template<std::size_t I, typename AST, typename Stack>
        using parse_t = typename parse<I, AST, Stack>::type;

        template<std::size_t I, typename AST, typename Stack>
        struct parse<I, AST, Stack, true>
        {
            using next_stack = pop<Stack>;
            using prev_token = token_t<I - 1>;
            using next_ast = ast::update_ast_t<top<Stack>, prev_token, AST>;

            using type = parse_t<I, next_ast, next_stack>;
        };

        // Base case - push the rule on the stack
        template<std::size_t I, typename Rule, typename AST, typename Stack>
        struct transition
        {
            using type = parse_t<I, AST, push<Stack, Rule>>;
        };

        // Don't push anything
        template<std::size_t I, typename AST, typename Stack>
        struct transition<I, grammar::ignore, AST, Stack>
        {
            using type = parse_t<I, AST, Stack>;
        };

        // Advance to the next token
        template<std::size_t I, typename AST, typename Stack>
        struct transition<I, grammar::advance, AST, Stack>
        {
            using type = parse_t<I + 1, AST, Stack>;
        };

        // Advance and also push the rest of the rules on the stack
        template<std::size_t I, typename AST, typename... Rules, typename... Rest>
        struct transition<I, stack<grammar::advance, Rules ...>, AST, stack<Rest ...>>
        {
            using type = parse_t<I + 1, AST, stack<Rules ..., Rest ...>>;
        };

        // Reject the input pattern
        template<std::size_t I, typename AST, typename Stack>
        struct transition<I, grammar::reject, AST, Stack>
        {
            using type = parser_result<ast::nothing, parsing::syntax_error<I>>;
        };

        // Accept the input pattern
        template<std::size_t I, typename AST, typename Stack>
        struct transition<I, grammar::accept, AST, Stack>
        {
            using type = parser_result<ast::preorder_indexing_t<0, top<AST>>, parsing::success>;
        };

        using result = parse_t<0, stack<>, stack<symbol::begin>>;
        using ast_type = typename result::ast_type;
        using status_type = typename result::status_type;

        static constexpr bool accepted = std::is_same_v<status_type, parsing::success>;
    };

    template<static_string pattern>
    using ast_of = typename parser<pattern>::ast_type;

    template<static_string pattern>
    using status_of = typename parser<pattern>::status_type;
}
#endif //MREGEX_PARSER_HPP