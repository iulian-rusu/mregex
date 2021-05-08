#ifndef CTR_SYMBOL_H
#define CTR_SYMBOL_H

/**
 * Namespace with symbols used by the LL(1) parser to generate the Abstract Syntax Tree
 */
namespace ctr::symbol
{
    // tag to mark semantic symbols
    struct semantic {};

    // semantic symbols for AST generation
    template<auto C>
    struct character: semantic {};

    struct epsilon: semantic {};

    template<typename ...>
    struct concatenation: semantic {};

    template<typename ...>
    struct alternation: semantic {};

    template<typename ...>
    struct star: semantic {};

    template<typename T>
    using optional = alternation<T, epsilon>;

    template<typename T>
    using plus = concatenation<T, star<T>>;

    // type trait to distinguish semantic symbols from other symbols
    template<typename Symbol>
    constexpr bool is_semantic_v = std::is_base_of_v<semantic, Symbol>;

    // non-semantic symbols for parser transitions
    struct start {};

    struct alt0 {};

    struct alt {};

    struct esc {};

    struct mod {};

    struct seq0 {};

    struct seq {};
}
#endif //CTR_SYMBOL_H