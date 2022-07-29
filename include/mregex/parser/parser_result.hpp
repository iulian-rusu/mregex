#ifndef MREGEX_PARSER_RESULT_HPP
#define MREGEX_PARSER_RESULT_HPP

#include <mregex/ast/astfwd.hpp>

namespace meta
{
    /**
     * Data type returned by the compile-time parser.
     *
     * @tparam AST  The generated Abstract Syntax Tree
     * @tparam S    A type that holds information about the exit status of the parser
     */
    template<typename AST, typename S>
    struct parser_result
    {
        using ast_type = AST;
        using status_type = S;
    };

    template<typename S>
    struct parser_result<symbol::empty, S>
    {
        using ast_type = ast::empty;
        using status_type = S;
    };

    /**
     * Namespace with the possible status types of the parser.
     */
    namespace parsing
    {
        // Parsing finished with success
        struct success : std::true_type {};

        // Parsing finished with syntax error
        template<std::size_t>
        struct syntax_error : std::false_type {};
    }
}
#endif //MREGEX_PARSER_RESULT_HPP