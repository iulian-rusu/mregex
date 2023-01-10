#ifndef MREGEX_AST_BUILD_HPP
#define MREGEX_AST_BUILD_HPP

#include <mregex/ast/astfwd.hpp>
#include <mregex/ast/traits.hpp>
#include <mregex/utility/type_sequence.hpp>

namespace meta::ast
{
    /**
     * Metafunction used to build the Abstract Syntax Tree.
     *
     * @tparam Symbol   The current symbol on the parsing stack
     * @tparam Token    The current token being parsed
     * @tparam Nodes    The stack with the AST nodes
     */
    template<typename Symbol, typename Token, typename Nodes>
    struct build;

    template<typename Symbol, typename Token, typename Nodes>
    using build_t = typename build<Symbol, Token, Nodes>::type;

    template<char C, typename Nodes>
    struct build<symbol::make_literal, symbol::character<C>, Nodes>
    {
        using type = push<Nodes, literal<C>>;
    };

    template<char C, typename Token, typename Nodes>
    struct build<symbol::push_literal<C>, Token, Nodes>
    {
        using type = push<Nodes, literal<C>>;
    };

    template<typename Token, typename Nodes>
    struct build<symbol::make_empty, Token, Nodes>
    {
        using type = push<Nodes, empty>;
    };

    template<typename Token, typename Nodes>
    struct build<symbol::make_digit, Token, Nodes>
    {
        using type = push<Nodes, digit>;
    };

    template<typename Token, typename Nodes>
    struct build<symbol::make_word, Token, Nodes>
    {
        using type = push<Nodes, word>;
    };

    template<typename Token, typename Nodes>
    struct build<symbol::make_word_boundary, Token, Nodes>
    {
        using type = push<Nodes, word_boundary>;
    };

    template<typename Token, typename Nodes>
    struct build<symbol::make_whitespace, Token, Nodes>
    {
        using type = push<Nodes, whitespace>;
    };

    template<typename Token, typename Nodes>
    struct build<symbol::make_lower, Token, Nodes>
    {
        using type = push<Nodes, lower>;
    };

    template<typename Token, typename Nodes>
    struct build<symbol::make_upper, Token, Nodes>
    {
        using type = push<Nodes, upper>;
    };

    template<typename Token, typename Nodes>
    struct build<symbol::make_hexa, Token, Nodes>
    {
        using type = push<Nodes, hexa>;
    };

    template<typename Token, typename Nodes>
    struct build<symbol::make_linebreak, Token, Nodes>
    {
        using type = push<Nodes, linebreak>;
    };

    template<typename Token, typename Nodes>
    struct build<symbol::make_wildcard, Token, Nodes>
    {
        using type = push<Nodes, wildcard>;
    };

    template<typename Token, typename Nodes>
    struct build<symbol::make_beginning_of_line, Token, Nodes>
    {
        using type = push<Nodes, beginning_of_line>;
    };

    template<typename Token, typename Nodes>
    struct build<symbol::make_end_of_line, Token, Nodes>
    {
        using type = push<Nodes, end_of_line>;
    };

    template<typename Token, typename Nodes>
    struct build<symbol::make_beginning_of_input, Token, Nodes>
    {
        using type = push<Nodes, beginning_of_input>;
    };

    template<typename Token, typename Nodes>
    struct build<symbol::make_end_of_input, Token, Nodes>
    {
        using type = push<Nodes, end_of_input>;
    };

    template<typename Token, typename First, typename... Rest>
    struct build<symbol::make_positive_lookahead, Token, type_sequence<First, Rest ...>>
    {
        using type = type_sequence<positive_lookahead<First>, Rest ...>;
    };

    template<typename Token, typename First, typename... Rest>
    struct build<symbol::make_negative_lookahead, Token, type_sequence<First, Rest ...>>
    {
        using type = type_sequence<negative_lookahead<First>, Rest ...>;
    };

    template<typename Token, typename First, typename... Rest>
    struct build<symbol::make_positive_lookbehind, Token, type_sequence<First, Rest ...>>
    {
        using type = type_sequence<positive_lookbehind<First>, Rest ...>;
    };

    template<typename Token, typename First, typename... Rest>
    struct build<symbol::make_negative_lookbehind, Token, type_sequence<First, Rest ...>>
    {
        using type = type_sequence<negative_lookbehind<First>, Rest ...>;
    };

    template<typename Token, typename First, typename... Rest>
    struct build<symbol::make_negated, Token, type_sequence<First, Rest ...>>
    {
        using type = type_sequence<negated<First>, Rest ...>;
    };

    template<match_mode Mode, typename Token, typename First, typename... Rest>
    struct build<symbol::make_star<Mode>, Token, type_sequence<First, Rest ...>>
    {
        using type = type_sequence<basic_star<Mode, First>, Rest ...>;
    };

    template<match_mode Mode, typename Token, typename First, typename... Rest>
    struct build<symbol::make_optional<Mode>, Token, type_sequence<First, Rest ...>>
    {
        using type = type_sequence<basic_optional<Mode, First>, Rest ...>;
    };

    template<match_mode Mode, typename Token, typename First, typename... Rest>
    struct build<symbol::make_plus<Mode>, Token, type_sequence<First, Rest ...>>
    {
        using type = type_sequence<basic_plus<Mode, First>, Rest ...>;
    };

    // Combine any two non-sequence symbols into a sequence
    template<typename Token, typename First, typename Second, typename... Rest>
    struct build<symbol::make_sequence, Token, type_sequence<First, Second, Rest ...>>
    {
        using type = type_sequence<sequence<Second, First>, Rest ...>;
    };

    // Add current symbol to previous sequence
    template<typename Token, typename First, typename... Second, typename... Rest>
    struct build<symbol::make_sequence, Token, type_sequence<First, sequence<Second ...>, Rest ...>>
    {
        using type = type_sequence<sequence<Second ..., First>, Rest ...>;
    };

    // Add previous symbol to current sequence
    template<typename Token, typename Second, typename... First, typename... Rest>
    struct build<symbol::make_sequence, Token, type_sequence<sequence<First ...>, Second, Rest ...>>
    {
        using type = type_sequence<sequence<Second, First ...>, Rest ...>;
    };

    // Concatenate two sequences
    template<typename Token, typename... First, typename... Second, typename... Rest>
    struct build<symbol::make_sequence, Token, type_sequence<sequence<First ...>, sequence<Second ...>, Rest ...>>
    {
        using type = type_sequence<sequence<Second ..., First ...>, Rest ...>;
    };

    template<typename Token, typename First, typename Second, typename... Rest>
    struct build<symbol::make_alternation, Token, type_sequence<First, Second, Rest ...>>
    {
        using type = type_sequence<alternation<Second, First>, Rest ...>;
    };

    template<typename Token, typename First, typename... Second, typename... Rest>
    struct build<symbol::make_alternation, Token, type_sequence<First, alternation<Second ...>, Rest ...>>
    {
        using type = type_sequence<alternation<Second ..., First>, Rest ...>;
    };

    template<typename Token, typename Name, typename First,  typename... Rest>
    struct build<symbol::make_capture<Name>, Token, type_sequence<First, Rest ...>>
    {
        static constexpr auto capture_count = count_captures<First, Rest ...>;

        using type = type_sequence<capture<capture_count + 1, Name, First>, Rest ...>;
    };

    template<std::size_t ID, typename Token, typename... Nodes>
    struct build<symbol::make_backref<ID>, Token, type_sequence<Nodes ...>>
    {
        using type = type_sequence<backref<ID>, Nodes ...>;
    };

    template<typename Name, typename Token, typename... Nodes>
    struct build<symbol::make_named_backref<Name>, Token, type_sequence<Nodes ...>>
    {
        using type = type_sequence<named_backref<Name>, Nodes ...>;
    };

    template<match_mode Mode, typename A, typename B, typename Token, typename First, typename... Rest>
    struct build<symbol::make_repetition<Mode, A, B>, Token, type_sequence<First, Rest ...>>
    {
        using type = type_sequence<basic_repetition<Mode, A, B, First>, Rest ...>;
    };

    // Set building rules
    template<typename Token, typename... Nodes>
    struct build<symbol::make_set, Token, type_sequence<Nodes ...>>
    {
        using type = type_sequence<nothing, Nodes ...>;
    };

    // Create set from current character
    template<char C, typename... Rest>
    struct build<symbol::make_set_from_current_char, symbol::character<C>, type_sequence<nothing, Rest ...>>
    {
        using type = type_sequence<set<literal<C>>, Rest ...>;
    };

    template<char C, typename... Nodes>
    struct build<symbol::make_set_from_current_char, symbol::character<C>, type_sequence<Nodes ...>>
    {
        using type = type_sequence<set<literal<C>>, Nodes ...>;
    };

    template<char C, typename... First, typename... Rest>
    struct build<symbol::make_set_from_current_char, symbol::character<C>, type_sequence<set<First ...>, Rest ...>>
    {
        using type = type_sequence<set<literal<C>, First ...>, Rest ...>;
    };

    // Create set from the last generated AST node
    template<typename Token, typename First, typename... Rest>
    struct build<symbol::make_set_from_stack, Token, type_sequence<First, nothing, Rest ...>>
    {
        using type = type_sequence<set<First>, Rest ...>;
    };

    template<typename Token, typename... First, typename... Rest>
    struct build<symbol::make_set_from_stack, Token, type_sequence<set<First ...>, nothing, Rest ...>>
    {
        using type = type_sequence<set<First ...>, Rest ...>;
    };

    template<typename Token, typename First, typename... Rest>
    struct build<symbol::make_set_from_stack, Token, type_sequence<First, Rest ...>>
    {
        using type = type_sequence<set<First>, Rest ...>;
    };

    template<typename Token, typename First, typename... Second, typename... Rest>
    struct build<symbol::make_set_from_stack, Token, type_sequence<First, set<Second ...>, Rest ...>>
    {
        using type = type_sequence<set<First, Second ...>, Rest ...>;
    };

    template<typename Token, typename... First, typename... Second, typename... Rest>
    struct build<symbol::make_set_from_stack, Token, type_sequence<set<First ...>, set<Second ...>, Rest ...>>
    {
        using type = type_sequence<set<First ..., Second ...>, Rest ...>;
    };

    // Create a range inside the set
    template<char A, char B, typename... Second, typename... Rest>
    struct build<symbol::make_range, symbol::character<A>, type_sequence<set<literal<B>, Second ...>, Rest ...>>
    {
        using type = type_sequence<set<range<B, A>, Second ...>, Rest ...>;
    };

    template<char C, typename First, typename... Second, typename... Rest>
    struct build<symbol::make_range, symbol::character<C>, type_sequence<set<First, Second ...>, Rest ...>>
    {
        using type = type_sequence<set<literal<C>, literal<'-'>, First, Second ...>, Rest ...>;
    };

    // Create a range from two last generated AST nodes in the set
    template<typename Token, char A, char B, typename... Second, typename... Rest>
    struct build<symbol::make_range_from_stack, Token, type_sequence<set<literal<A>, literal<B>, Second ...>, Rest ...>>
    {
        using type = type_sequence<set<range<B, A>, Second ...>, Rest ...>;
    };

    template<typename Token, char C, typename First, typename... Second, typename... Rest>
    struct build<symbol::make_range_from_stack, Token, type_sequence<set<literal<C>, First, Second ...>, Rest ...>>
    {
        using type = type_sequence<set<literal<C>, literal<'-'>, First, Second ...>, Rest ...>;
    };
}
#endif //MREGEX_AST_BUILD_HPP