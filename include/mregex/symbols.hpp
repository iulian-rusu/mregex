#ifndef MREGEX_SYMBOLS_HPP
#define MREGEX_SYMBOLS_HPP

#include <mregex/utility/static_string.hpp>
#include <mregex/match_mode.hpp>

namespace meta::symbol
{
    // Symbols for parsing tokens
    template<char C>
    struct character {};

    struct empty {};

    template<char C>
    struct expect {};

    // Symbols for parsing basic operators
    struct begin {};
    struct esc {};
    struct seq {};
    struct alt {};
    struct alt_seq {};
    struct mod {};

    // Symbols for parsing groups
    struct group_begin_or_mod {};
    struct group_mod {};
    struct group_mod_less {};
    struct group_begin {};

    // Symbols for parsing sets
    struct set_begin_or_neg {};
    struct set_begin {};
    struct set_seq {};
    struct set_esc {};
    struct set_range_begin {};
    struct set_range_seq {};
    struct set_range_esc {};

    // Symbols for parsing quantifiers
    struct unquantifiable {};
    struct quantifier_begin {};
    struct quantifier_inf {};

    template<std::size_t N>
    struct quantifier_value {};

    template<typename A, typename B>
    struct quantifier_values {};

    template<typename Action>
    struct quantifier_mod {};

    // Symbols for named AST nodes
    template<static_string Name>
    struct name
    {
        static constexpr auto value = static_cast<std::string_view>(Name);
    };

    struct unnamed {};

    // Symbol for parsing backreferences
    template<std::size_t ID>
    struct backref_id {};

    struct backref_name_begin {};

    template<char... Chars>
    struct backref_name_seq {};

    // Symbols for parsing named captures
    template<char... Chars>
    struct capture_name_seq {};

    // Tag type for symbols which require a semantic action
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
    struct make_hexa : semantic_action {};
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

    template<match_mode Mode, typename A, typename B>
    struct make_repetition : semantic_action {};

    template<match_mode Mode>
    struct make_star : semantic_action {};

    template<match_mode Mode>
    struct make_plus : semantic_action {};

    template<match_mode Mode>
    struct make_optional : semantic_action {};

    template<std::size_t ID>
    struct make_backref : semantic_action {};

    template<typename Name>
    struct make_named_backref : semantic_action {};

    struct make_positive_lookahead : semantic_action {};
    struct make_negative_lookahead : semantic_action {};
    struct make_positive_lookbehind : semantic_action {};
    struct make_negative_lookbehind : semantic_action {};

    /**
     * Type trait used to distinguish semantic action symbols from other symbols.
     */
    template<typename Symbol>
    inline constexpr bool is_semantic_action = std::is_base_of_v<semantic_action, Symbol>;

    /**
     * Type trait used to check if a type is a symbolic quantifier.
     */
    template<typename Symbol>
    inline constexpr bool is_quantifier = false;

    template<std::size_t N>
    inline constexpr bool is_quantifier<quantifier_value<N>> = true;

    template<>
    inline constexpr bool is_quantifier<quantifier_inf> = true;

    template<typename Symbol>
    concept quantifier = is_quantifier<Symbol>;

    /**
     * Predicate that checks if a symbolic quantifier is equivalent to infinity.
     */
    template<quantifier>
    inline constexpr bool is_infinity = false;

    template<>
    inline constexpr bool is_infinity<quantifier_inf> = true;

    template<typename Symbol>
    concept finite_quantifier = quantifier<Symbol> && !is_infinity<Symbol>;

    /**
     * Predicate that checks if a symbolic quantifier is equivalent to 0.
     */
    template<quantifier Symbol>
    inline constexpr bool is_zero = false;

    template<>
    inline constexpr bool is_zero<quantifier_value<0>> = true;

    /**
     * Predicate that checks if a pair of quantifiers for a valid range (interval).
     */
    template<quantifier A, quantifier B>
    inline constexpr bool is_valid_range = false;

    template<std::size_t A, std::size_t B>
    inline constexpr bool is_valid_range<quantifier_value<A>, quantifier_value<B>> = A <= B;

    template<std::size_t N>
    inline constexpr bool is_valid_range<quantifier_value<N>, quantifier_inf> = true;

    /**
     * Metafunction that projects a finite symbolic quantifier to its numerical value.
     */
    template<finite_quantifier Symbol>
    inline constexpr std::size_t get_value = {};

    template<std::size_t N>
    inline constexpr std::size_t get_value<quantifier_value<N>> = N;

    /**
     * Predicate that checks if a symbolic quantifier is equal to some integral value.
     */
    template<quantifier Symbol>
    inline constexpr auto equals = [](std::size_t) noexcept { return false; };

    template<std::size_t N>
    inline constexpr auto equals<quantifier_value<N>> = [](std::size_t value) noexcept { return N == value; };

    /**
     * Metafunction used to decrement symbolic quantifiers.
     */
    template<quantifier Symbol>
    struct decrement;

    template<std::size_t N>
    requires (N > 0)
    struct decrement<quantifier_value<N>> : std::type_identity<quantifier_value<N - 1>> {};

    template<>
    struct decrement<quantifier_inf> : std::type_identity<quantifier_inf> {};

    template<quantifier Symbol>
    using decrement_t = typename decrement<Symbol>::type;

    /**
     * Metafunction that handles subtraction of symbolic quantifiers.
     */
    template<quantifier A, quantifier B>
    struct subtract;

    template<std::size_t A, std::size_t B>
    requires (A >= B)
    struct subtract<quantifier_value<A>, quantifier_value<B>> : std::type_identity<quantifier_value<A - B>> {};

    template<std::size_t N>
    struct subtract<quantifier_inf, quantifier_value<N>> : std::type_identity<quantifier_inf> {};

    template<quantifier A, quantifier B>
    using subtract_t = typename subtract<A, B>::type;

    /**
     * Metafunction that converts a pack of characters to a symbolic name type.
     *
     * @note The sizeof... (Chars) is technically redundant but GCC 11.1 cannot deduce the size.
     */
    template<char... Chars>
    using make_name = name<static_string<sizeof... (Chars)>{{Chars ..., '\0'}}>;
}
#endif //MREGEX_SYMBOLS_HPP