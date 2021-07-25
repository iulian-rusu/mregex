#ifndef CX_ASTFWD_H
#define CX_ASTFWD_H

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

    template<std::size_t, typename>
    struct repeated;

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
    struct atomic;

    template<std::size_t, typename>
    struct capturing;
}
#endif //CX_ASTFWD_H
