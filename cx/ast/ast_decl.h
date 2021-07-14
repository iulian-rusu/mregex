#ifndef CX_AST_DECL_H
#define CX_AST_DECL_H

namespace cx
{
    /**
     * Forward declaration of basic AST node types for type resolution
     */

    template<typename, typename ...>
    struct sequence;

    template<typename, typename ...>
    struct alternation;

    template<typename>
    struct star;

    struct terminal;

    struct epsilon : terminal;

    struct null : terminal;

    struct beginning : terminal;

    struct ending : terminal;

    template<auto>
    struct character : terminal;

    struct whitespace : terminal;

    struct wildcard : terminal;

    template<auto, auto>
    struct range : terminal;

    template<std::size_t>
    struct backref : terminal;

    template<typename>
    struct negated;

    template<typename>
    struct atomic;

    template<std::size_t, typename>
    struct capturing;
}
#endif //CX_AST_DECL_H
