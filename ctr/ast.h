#ifndef CTR_AST_H
#define CTR_AST_H

/**
 * File with the building blocks of the Abstract Syntax Tree
 */
namespace ctr
{
    struct epsilon {};

    template<auto C>
    struct character {};

    template<typename ...>
    struct sequence {};

    template<typename ...>
    struct alternation {};

    template<typename ...>
    struct star {};

    template<typename T>
    using optional = alternation<T, epsilon>;

    template<typename T>
    using plus = sequence<T, star<T>>;

    struct alnum {};

    struct digit {};

    struct word {};

    struct whitespace {};

    struct lower {};

    struct upper {};

    struct hexa {};

    template<typename S>
    struct negated {};

    template<typename S>
    struct capturing {};
}
#endif //CTR_AST_H
