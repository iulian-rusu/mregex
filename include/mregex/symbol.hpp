#ifndef MREGEX_SYMBOL_HPP
#define MREGEX_SYMBOL_HPP

#include <type_traits>

namespace meta::symbol
{
    // Basic symbols
    struct begin {};

    struct esc {};

    struct mod {};

    struct seq {};

    struct alt {};

    struct alt_seq {};

    // Symbols for parsing groups
    struct group_begin_or_mod {};

    struct group_mod {};

    struct group_begin {};

    // Symbols for parsing sets
    struct set_begin_or_neg {};

    struct set_begin {};

    struct set_seq {};

    struct set_esc {};

    struct set_range_begin {};

    struct set_range_seq {};

    struct set_range_esc {};

    // Symbols for tokens
    template<auto>
    struct character {};

    struct epsilon {};

    template<auto>
    struct expect {};

    // Symbol for parsing a backreference ID
    template<std::size_t>
    struct backref_id {};

    // Symbols for parsing quantifiers
    struct quantifier_begin {};

    template<std::size_t>
    struct quantifier_value {};

    struct quantifier_inf {};

    template<typename, typename>
    struct quantifier_values {};

    // Symbols that indicate an action which updates the AST structure
    struct ast_update {};

    struct make_literal : ast_update {};

    template<auto>
    struct push_literal : ast_update {};

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

    struct make_linebreak : ast_update {};

    struct make_wildcard : ast_update {};

    struct make_beginning : ast_update {};

    struct make_ending : ast_update {};

    struct make_positive_lookahead : ast_update {};

    struct make_negative_lookahead : ast_update {};

    struct make_negated : ast_update {};

    struct make_capture : ast_update {};

    struct make_set : ast_update {};

    struct make_set_from_current_char : ast_update {};

    struct make_set_from_stack : ast_update {};

    struct make_range : ast_update {};

    struct make_range_from_stack : ast_update {};

    template<std::size_t>
    struct make_backref : ast_update {};

    /**
     * Type trait to distinguish AST update symbols from other symbols.
     */
    template<typename Symbol>
    inline constexpr bool is_ast_update_v = std::is_base_of_v<ast_update, Symbol>;
}
#endif //MREGEX_SYMBOL_HPP