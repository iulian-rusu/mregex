#ifndef CTR_SYMBOL_H
#define CTR_SYMBOL_H

/**
 * Namespace with symbols used by the LL(1) parser to generate the Abstract Syntax Tree
 */
namespace ctr::symbol
{
    // tag to mark terminal symbols
    struct terminal {};

    // terminal symbols
    template<auto C>
    struct character: terminal
    {
        static constexpr auto value = C;
    };

    struct epsilon: terminal {};

    template<typename ...>
    struct concatenation: terminal {};

    template<typename ...>
    struct alternation: terminal {};

    template<typename ...>
    struct star: terminal {};

    template<typename T>
    using optional = alternation<T, epsilon>;

    template<typename T>
    using plus = concatenation<T, star<T>>;

    // type trait to distinguish terminal symbols from other symbols
    template<typename Symbol>
    constexpr bool is_terminal_v = std::is_base_of_v<terminal, Symbol>;

    // non-terminal symbols
    struct start {};

    struct alt0 {};

    struct alt {};

    struct esc {};

    struct mod {};

    struct seq0 {};

    struct seq {};
}
#endif //CTR_SYMBOL_H