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

    struct epsilon;

    struct null;

    struct beginning;

    struct ending;

    template<auto>
    struct character;

    struct whitespace;

    struct wildcard;

    template<auto, auto>
    struct range;

    template<std::size_t>
    struct backref;

    template<typename>
    struct negated;

    template<typename>
    struct atomic;

    template<std::size_t, typename>
    struct capturing;
}
#endif //CX_AST_DECL_H
