#ifndef META_SYMBOL_HPP
#define META_SYMBOL_HPP

namespace meta::symbol
{
    // Non-terminal symbols
    struct epsilon {};

    struct start {};

    struct alt {};

    struct esc {};

    struct mod {};

    struct seq0 {};

    struct seq {};

    // Symbols for parsing capturing groups
    struct capture_begin {};

    struct capture_begin_no_mod {};

    struct capture_mod {};

    // Symbol that requires a specific character to appear
    template<auto>
    struct expect {};

    // Wrapper for marking a non-capturing group
    template<typename>
    struct captureless_wrapper {};

    // Symbols for parsing sets
    struct set_begin {};

    struct set_begin_no_neg {};

    struct set_seq {};

    struct set_seq0 {};

    struct set_esc {};

    struct set_range_start {};

    struct set_range_esc {};

    // Symbol for parsing a backreference ID
    template<std::size_t>
    struct backref_id {};

    // Symbols for parsing a quantifier value
    struct quantifier_begin {};

    template<std::size_t>
    struct quantifier_value {};

    struct quantifier_inf {};

    template<typename, typename>
    struct quantifier_values {};

    // Tag for symbols that require AST updates
    struct ast_update {};

    struct make_char : ast_update {};

    template<auto>
    struct push_char : ast_update {};

    struct make_sequence : ast_update {};

    struct make_alternation : ast_update {};

    struct make_star : ast_update {};

    template<typename, typename>
    struct make_repetition : ast_update {};

    struct make_optional : ast_update {};

    struct make_plus : ast_update {};

    struct make_alpha : ast_update {};

    struct make_digit : ast_update {};

    struct make_word : ast_update {};

    struct make_whitespace : ast_update  {};

    struct make_lower : ast_update  {};

    struct make_upper : ast_update  {};

    struct make_hexa : ast_update {};

    struct make_wildcard : ast_update {};

    struct make_beginning : ast_update {};

    struct make_ending : ast_update {};

    struct make_negated : ast_update {};

    struct make_captureless : ast_update{};

    struct make_capturing : ast_update {};

    struct make_set : ast_update {};

    struct make_set_from_current_char : ast_update {};

    struct make_set_from_stack : ast_update {};

    struct make_range : ast_update {};

    struct make_range_from_stack : ast_update {};

    template<std::size_t>
    struct make_backref : ast_update {};

    // Type trait to distinguish AST update symbols from other symbols
    template<typename Symbol>
    inline constexpr bool is_ast_update_v = std::is_base_of_v<ast_update, Symbol>;
}
#endif //META_SYMBOL_HPP