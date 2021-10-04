#ifndef MREGEX_PARSER_RESULT_HPP
#define MREGEX_PARSER_RESULT_HPP

#include <mregex/utility/stack.hpp>

namespace meta
{
    /**
     * Data type returned by the parsing metafunction.
     *
     * @tparam AST  The generated Abstract Syntax Tree
     * @tparam E    Type that indicates the presence of an error during parsing
     */
    template<typename AST, typename E>
    struct parser_result
    {
        using ast_type = AST;
        using error_type = E;
    };

    template<typename E>
    struct parser_result<empty_stack_marker, E>
    {
        using ast_type = ast::epsilon;
        using error_type = E;
    };

    /**
     * Type returned when the parser finds a syntax error.
     *
     * @tparam I    The position at which the parsing stopped
     */
    template<std::size_t I>
    struct syntax_error_at_position;

    /**
     * Helper metafunction that causes a compilation error when instantiated with an error type.
     */
    template<typename E>
    struct identify_error
    {
        static constexpr auto error_type = E{};
    };
}
#endif //MREGEX_PARSER_RESULT_HPP