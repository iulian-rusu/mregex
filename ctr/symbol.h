#ifndef CTR_SYMBOL_H
#define CTR_SYMBOL_H

/**
 * Namespace with symbols used by the LL(1) parser to generate the Abstract Syntax Tree
 */
namespace ctr::symbol
{
    // non-action symbols
    struct epsilon {};

    struct start {};

    struct alt0 {};

    struct alt {};

    struct esc {};

    struct mod {};

    struct seq0 {};

    struct seq {};

    // tag to mark action symbols
    struct action {};

    // action symbols
    struct character: action {};

    struct sequence: action {};

    struct alternation: action {};

    struct star: action {};

    struct optional: action {};

    struct plus: action {};

    struct alnum: action {};

    struct digit: action {};

    // type trait to distinguish action symbols from other symbols
    template<typename Symbol>
    constexpr bool is_action_v = std::is_base_of_v<action, Symbol>;
}
#endif //CTR_SYMBOL_H