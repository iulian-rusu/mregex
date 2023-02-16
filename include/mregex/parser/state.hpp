#ifndef MREGEX_PARSER_STATE_HPP
#define MREGEX_PARSER_STATE_HPP

#include <mregex/ast/astfwd.hpp>

namespace meta
{
    /**
     * Types that define the possible verdicts after parsing is finished.
     */
    namespace parsing
    {
        // Parsing finished with success
        struct success : std::true_type {};

        // Parsing finished with syntax error
        template<std::size_t>
        struct syntax_error : std::false_type {};
    }

    /**
    * Data type returned by the compile-time parser.
    *
    * @tparam AST       The generated Abstract Syntax Tree
    * @tparam Verdict   Type that holds information about the verdict of the parser
    */
    template<typename AST, typename Verdict>
    struct parser_result
    {
        using ast_type = AST;
        using verdict = Verdict;
    };

    template<typename Verdict>
    struct parser_result<symbol::empty, Verdict>
    {
        using ast_type = ast::empty;
        using verdict = Verdict;
    };

    /**
     * Metacontainer that captures the state of the parser at a given parsing step.
     *
     * @tparam Parser   The parser metafunction type
     * @tparam I        The current position in the input
     * @tparam Nodes    The stack with the AST nodes
     * @tparam Symbols  The stack with the current parsing symbols
     */
    template<typename Parser, std::size_t I, typename Nodes, typename Symbols>
    struct parser_state {};

    namespace detail
    {
        /**
         * To minimize template recursion depth, parsing is done by advancing
         * the parser state using a fold expression.
         */

        template<typename Parser, std::size_t I, typename Nodes, typename Symbols>
        constexpr auto operator<<(parser_state<Parser, I, Nodes, Symbols>, std::size_t) noexcept
        {
            return typename Parser::template parse_t<I, Nodes, Symbols>{};
        }

        template<typename AST, typename Verdict>
        constexpr auto operator<<(parser_result<AST, Verdict>, std::size_t) noexcept
        {
            return parser_result<AST, Verdict>{};
        }

        template<typename State, std::size_t... Indices>
        static constexpr auto advance_parser_state(State initial_state, std::index_sequence<Indices ...>) noexcept
        {
            return (initial_state << ... << Indices);
        }
    }

    /**
     * Computes the final parser state starting from an initial state and advancing it N times.
     */
    template<typename State, std::size_t N>
    using final_parser_state = decltype(detail::advance_parser_state(State{}, std::make_index_sequence<N + 1>{}));
}
#endif //MREGEX_PARSER_STATE_HPP