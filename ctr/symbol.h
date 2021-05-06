#ifndef CTR_SYMBOL_H
#define CTR_SYMBOL_H

/**
 * Namespace with symbols used by the LL(1) parser to generate the Abstract Syntax Tree
 */
namespace ctr::symbol
{
    // symbols for AST
    template<auto C>
    struct character{};

    struct epsilon {};

    template<typename ...>
    struct concatenation {};

    template<typename ...>
    struct alternation {};

    template<typename ...>
    struct star {};

    template<typename T>
    using optional = alternation<T, epsilon>;

    template<typename T>
    using plus = concatenation<T, star<T>>;

    // symbols for parser transitions
    struct start {};

    struct alt0 {};

    struct alt {};

    struct esc {};

    struct mod {};

    struct seq0 {};

    struct seq {};
}
#endif //CTR_SYMBOL_H