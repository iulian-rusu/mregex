#ifndef CX_SYMBOL_H
#define CX_SYMBOL_H

/**
 * Namespace with symbols used by the LL(1) parser to generate the Abstract Syntax Tree
 */
namespace cx::symbol
{
    // Non-terminal symbols
    struct epsilon {};

    struct start {};

    struct alt0 {};

    struct alt {};

    struct esc {};

    struct mod {};

    struct seq0 {};

    struct seq {};

    struct captureless_begin {};

    struct capture_mod {};

    struct set_begin {};

    struct set_begin_no_neg {};

    struct set_seq {};

    struct set_seq0 {};

    struct set_esc {};

    struct set_range_start {};

    struct set_range_esc {};

    template<std::size_t ID>
    struct backref_id {};

    // Symbols that require AST updates
    struct ast_update {};

    struct make_char: ast_update {};

    struct make_sequence: ast_update {};

    struct make_alternation: ast_update {};

    struct make_star: ast_update {};

    struct make_optional: ast_update {};

    struct make_plus: ast_update {};

    struct make_alnum: ast_update {};

    struct make_digit: ast_update {};

    struct make_word: ast_update {};

    struct make_whitespace: ast_update  {};

    struct make_lower: ast_update  {};

    struct make_upper: ast_update  {};

    struct make_hexa: ast_update {};

    struct make_wildcard: ast_update {};

    struct make_beginning: ast_update {};

    struct make_ending: ast_update {};

    struct make_negated: ast_update {};

    struct make_captureless: ast_update{};

    struct make_capturing: ast_update {};

    struct make_set: ast_update {};

    struct make_set_from_current_char: ast_update {};

    struct make_set_from_stack: ast_update {};

    struct make_range: ast_update {};

    struct make_range_from_stack: ast_update {};

    template<std::size_t ID>
    struct make_backref: ast_update {};

    // Type trait to distinguish ast_update symbols from other symbols
    template<typename Symbol>
    constexpr bool is_ast_update_v = std::is_base_of_v<ast_update, Symbol>;
}
#endif //CX_SYMBOL_H