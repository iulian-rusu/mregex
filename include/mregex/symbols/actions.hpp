#ifndef MREGEX_SYMBOLS_ACTIONS_HPP
#define MREGEX_SYMBOLS_ACTIONS_HPP

#include <mregex/symbols/quantifiers.hpp>
#include <mregex/utility/enums.hpp>

namespace meta::symbol
{
    /**
     * Symbols which indicate a semantic action for the parser.
     * Typically they are used to update the state of the AST during parsing.
     */

    // Base type for symbols which require a semantic action
    struct semantic_action {};

    struct make_sequence : semantic_action {};
    struct make_alternation : semantic_action {};
    struct make_set : semantic_action {};
    struct make_set_from_current_char : semantic_action {};
    struct make_set_from_stack : semantic_action {};
    struct make_range : semantic_action {};
    struct make_range_from_stack : semantic_action {};
    struct make_digit : semantic_action {};
    struct make_lower : semantic_action {};
    struct make_upper : semantic_action {};
    struct make_word : semantic_action {};
    struct make_linebreak : semantic_action {};
    struct make_negated : semantic_action {};
    struct make_empty : semantic_action {};
    struct make_beginning_of_line : semantic_action {};
    struct make_beginning_of_input : semantic_action {};
    struct make_end_of_line : semantic_action {};
    struct make_end_of_input : semantic_action {};
    struct make_word_boundary : semantic_action {};
    struct make_whitespace : semantic_action {};
    struct make_wildcard : semantic_action {};
    struct make_literal : semantic_action {};

    template<char C>
    struct push_literal : semantic_action {};

    template<typename Name>
    struct make_capture : semantic_action {};

    template<match_mode Mode, finite_quantifier A, quantifier B>
    struct make_repetition : semantic_action {};

    template<match_mode Mode>
    using make_star = make_repetition<Mode, quantifier_value<0>, infinity>;

    template<match_mode Mode>
    using make_plus = make_repetition<Mode, quantifier_value<1>, infinity>;

    template<match_mode Mode>
    using make_optional = make_repetition<Mode, quantifier_value<0>, quantifier_value<1>>;

    template<std::size_t ID>
    struct make_backref : semantic_action {};

    template<typename Name>
    struct make_named_backref : semantic_action {};

    template<assertion_mode Mode,  lookaround_direction Direction>
    struct make_lookaround : semantic_action {};

    /**
     * Type trait used to distinguish semantic action symbols from other symbols.
     */
    template<typename Symbol>
    inline constexpr bool is_semantic_action = std::is_base_of_v<semantic_action, Symbol>;
}
#endif //MREGEX_SYMBOLS_ACTIONS_HPP