#ifndef MREGEX_PARSER_HPP
#define MREGEX_PARSER_HPP

#include <mregex/ast/build.hpp>
#include <mregex/ast/index.hpp>
#include <mregex/grammar/grammar.hpp>
#include <mregex/parser/lexer.hpp>
#include <mregex/parser/state.hpp>

namespace meta
{
    /**
     * Compile-time parser using the LL(1) algorithm.
     *
     * @tparam Pattern  The input string to be parsed
     */
    template<static_string Pattern>
    struct parser
    {
        /**
         * Metafunction used to capture the current state of the parser.
         */
        template<std::size_t I, typename Nodes, typename Symbols>
        using state_t = parser_state<parser<Pattern>, I, Nodes, Symbols>;

        /**
         * Metafunction used to extract tokens (characters or empty tokens) from the input pattern.
         */
        template<std::size_t I>
        using token_t = typename lexer<Pattern>::template token_t<I>;

        /**
         * Metafunction that models the transition of the parser automaton.
         *
         * @tparam I            The current position in the input
         * @tparam NewSymbols   The new parsing symbols generated by the grammar
         * @tparam Nodes        The stack with the AST nodes
         * @tparam Symbols      The stack with the current parsing symbols
         */
        template<std::size_t I, typename NewSymbols, typename Nodes, typename Symbols>
        struct transition;

        template<std::size_t I, typename NewSymbols, typename Nodes, typename Symbols>
        using transition_t = typename transition<I, NewSymbols, Nodes, Symbols>::type;

        /**
         * Main metafunction used to parse the pattern.
         *
         * @tparam I        The current position in the input
         * @tparam Nodes    The stack with the AST nodes
         * @tparam Symbols  The stack with the current parsing symbols
         */
        template<std::size_t I, typename Nodes, typename Symbols, bool = symbol::is_semantic_action<top<Symbols>>>
        struct parse;

        template<std::size_t I, typename Nodes, typename Symbols>
        using parse_t = typename parse<I, Nodes, Symbols>::type;

        template<std::size_t I, typename Nodes, typename Symbols>
        struct parse<I, Nodes, Symbols, true>
        {
            using next_nodes = ast::build_t<top<Symbols>, token_t<I - 1>, Nodes>;
            using type = parse_t<I, next_nodes, pop<Symbols>>;
        };

        template<std::size_t I, typename Nodes, typename Symbols>
        struct parse<I, Nodes, Symbols, false>
        {
            using next_symbols = grammar::rule_t<top<Symbols>, token_t<I>>;
            using type = transition_t<I, next_symbols, Nodes, pop<Symbols>>;
        };

        // Base case - push the symbols on the stack
        template<std::size_t I, typename NewSymbols, typename Nodes, typename Symbols>
        struct transition
        {
            using type = parse_t<I, Nodes, push<Symbols, NewSymbols>>;
        };

        // Don't push anything
        template<std::size_t I, typename Nodes, typename Symbols>
        struct transition<I, grammar::ignore, Nodes, Symbols>
        {
            using type = parse_t<I, Nodes, Symbols>;
        };

        // Advance to the next token
        template<std::size_t I, typename Nodes, typename Symbols>
        struct transition<I, grammar::advance, Nodes, Symbols>
        {
            using type = state_t<I + 1, Nodes, Symbols>;
        };

        // Advance and also push the remaining symbols on the stack
        template<std::size_t I, typename Nodes, typename... NewSymbols, typename... Symbols>
        struct transition<I, type_sequence<grammar::advance, NewSymbols ...>, Nodes, type_sequence<Symbols ...>>
        {
            using type = state_t<I + 1, Nodes, type_sequence<NewSymbols ..., Symbols ...>>;
        };

        // Reject the input pattern
        template<std::size_t I, typename Nodes, typename Symbols>
        struct transition<I, grammar::reject, Nodes, Symbols>
        {
            using type = parser_result<ast::nothing, parsing::syntax_error<I>>;
        };

        // Accept the input pattern
        template<std::size_t I, typename Nodes, typename Symbols>
        struct transition<I, grammar::accept, Nodes, Symbols>
        {
            using type = parser_result<ast::preorder_index_t<0, top<Nodes>>, parsing::success>;
        };

        using initial_state = state_t<0, type_sequence<>, type_sequence<symbol::begin>>;
        using result = next_state_t<initial_state, Pattern.length()>;
        using ast_type = typename result::ast_type;
        using verdict_type = typename result::verdict_type;

        static constexpr bool accepted = std::is_same_v<verdict_type, parsing::success>;
    };

    template<static_string Pattern>
    using ast_for = typename parser<Pattern>::ast_type;

    template<static_string Pattern>
    using parser_verdict_for = typename parser<Pattern>::verdict_type;
}
#endif //MREGEX_PARSER_HPP