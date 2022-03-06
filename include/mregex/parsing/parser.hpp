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
         *
         * @tparam I        The current position in the input
         * @tparam Rule     The current grammar rule
         * @tparam AST      The stack with the current state of the AST
         * @tparam Symbols  The stack with the current parsing symbols
         */
        template<std::size_t I, typename Rule, typename AST, typename Symbols>
        struct transition;

        template<std::size_t I, typename Rule, typename AST, typename Symbols>
        using transition_t = typename transition<I, Rule, AST, Symbols>::type;

        /**
         * Main metafunction used to parse the pattern.
         *
         * @tparam I        The current position in the input
         * @tparam AST      The stack with the current state of the AST
         * @tparam Symbols  The stack with the current parsing symbols
         */
        template<std::size_t I, typename AST, typename Symbols, bool = symbol::is_ast_update_v<top<Symbols>>>
        struct parse
        {
            using next_symbols = grammar::rule_t<top<Symbols>,  token_t<I>>;
            using type = transition_t<I, next_symbols, AST, pop<Symbols>>;
        };

        template<std::size_t I, typename AST, typename Symbols>
        using parse_t = typename parse<I, AST, Symbols>::type;

        template<std::size_t I, typename AST, typename Symbols>
        struct parse<I, AST, Symbols, true>
        {
            using next_ast = ast::build_t<top<Symbols>, token_t<I - 1>, AST>;
            using type = parse_t<I, next_ast, pop<Symbols>>;
        };

        // Base case - push the symbols on the stack
        template<std::size_t I, typename Symbols, typename AST, typename Stack>
        struct transition
        {
            using type = parse_t<I, AST, push<Stack, Symbols>>;
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