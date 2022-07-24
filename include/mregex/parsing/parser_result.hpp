#ifndef MREGEX_PARSER_RESULT_HPP
#define MREGEX_PARSER_RESULT_HPP

#include <mregex/ast/astfwd.hpp>
#include <mregex/utility/type_sequence.hpp>

namespace meta
{
    /**
     * Data type returned by the parsing metafunction.
     *
     * @tparam AST  The generated Abstract Syntax Tree
     * @tparam S    Type that holds information about the exit status of parsing
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
}
#endif //MREGEX_PARSER_RESULT_HPP