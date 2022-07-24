#ifndef MREGEX_AST_BUILDER_HPP
#define MREGEX_AST_BUILDER_HPP

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

    template<char A, typename T, typename Nodes>
    struct build<symbol::push_literal<A>, T, Nodes>
    {
        using type = push<Nodes, literal<A>>;
    };

    template<typename T, typename Nodes>
    struct build<symbol::make_empty, T, Nodes>
    {
        using type = push<Nodes, empty>;
    };

    template<typename T, typename Nodes>
    struct build<symbol::make_digit, T, Nodes>
    {
        using type = push<Nodes, digit>;
    };

    template<typename T, typename Nodes>
    struct build<symbol::make_word, T, Nodes>
    {
        using type = push<Nodes, word>;
    };

    template<typename T, typename Nodes>
    struct build<symbol::make_word_boundary, T, Nodes>
    {
        using type = push<Nodes, word_boundary>;
    };

    template<typename T, typename Nodes>
    struct build<symbol::make_whitespace, T, Nodes>
    {
        using type = push<Nodes, whitespace>;
    };

    template<typename T, typename Nodes>
    struct build<symbol::make_lower, T, Nodes>
    {
        using type = push<Nodes, lower>;
    };

    template<typename T, typename Nodes>
    struct build<symbol::make_upper, T, Nodes>
    {
        using type = push<Nodes, upper>;
    };

    template<typename T, typename Nodes>
    struct build<symbol::make_hexa, T, Nodes>
    {
        using type = push<Nodes, hexa>;
    };

    template<typename T, typename Nodes>
    struct build<symbol::make_linebreak, T, Nodes>
    {
        using type = push<Nodes, linebreak>;
    };

    template<typename T, typename Nodes>
    struct build<symbol::make_wildcard, T, Nodes>
    {
        using type = push<Nodes, wildcard>;
    };

    template<typename T, typename Nodes>
    struct build<symbol::make_beginning, T, Nodes>
    {
        using type = push<Nodes, beginning>;
    };

    template<typename T, typename Nodes>
    struct build<symbol::make_end, T, Nodes>
    {
        using type = push<Nodes, end>;
    };

    template<typename T, typename Nodes>
    struct build<symbol::make_beginning_of_input, T, Nodes>
    {
        using type = push<Nodes, beginning_of_input>;
    };

    template<typename T, typename Nodes>
    struct build<symbol::make_end_of_input, T, Nodes>
    {
        using type = push<Nodes, end_of_input>;
    };

    template<typename T, typename First, typename... Rest>
    struct build<symbol::make_positive_lookahead, T, type_sequence<First, Rest ...>>
    {
        using type = type_sequence<positive_lookahead<First>, Rest ...>;
    };

    template<typename T, typename First, typename... Rest>
    struct build<symbol::make_negative_lookahead, T, type_sequence<First, Rest ...>>
    {
        using type = type_sequence<negative_lookahead<First>, Rest ...>;
    };

    template<typename T, typename First, typename... Rest>
    struct build<symbol::make_positive_lookbehind, T, type_sequence<First, Rest ...>>
    {
        using type = type_sequence<positive_lookbehind<First>, Rest ...>;
    };

    template<typename T, typename First, typename... Rest>
    struct build<symbol::make_negative_lookbehind, T, type_sequence<First, Rest ...>>
    {
        using type = type_sequence<negative_lookbehind<First>, Rest ...>;
    };

    template<typename T, typename First, typename... Rest>
    struct build<symbol::make_negated, T, type_sequence<First, Rest ...>>
    {
        using type = type_sequence<negated<First>, Rest ...>;
    };

    template<match_mode Mode, typename T, typename First, typename... Rest>
    struct build<symbol::make_star<Mode>, T, type_sequence<First, Rest ...>>
    {
        using type = type_sequence<basic_star<Mode, First>, Rest ...>;
    };

    template<match_mode Mode, typename T, typename First, typename... Rest>
    struct build<symbol::make_optional<Mode>, T, type_sequence<First, Rest ...>>
    {
        using type = type_sequence<basic_optional<Mode, First>, Rest ...>;
    };

    template<match_mode Mode, typename T, typename First, typename... Rest>
    struct build<symbol::make_plus<Mode>, T, type_sequence<First, Rest ...>>
    {
        using type = type_sequence<basic_plus<Mode, First>, Rest ...>;
    };


    // Combine any two non-sequence symbols into a sequence
    template<typename T, typename First, typename Second, typename... Rest>
    struct build<symbol::make_sequence, T, type_sequence<First, Second, Rest ...>>
    {
        using type = type_sequence<sequence<Second, First>, Rest ...>;
    };

    // Add current symbol to previous sequence
    template<typename T, typename First, typename... Second, typename... Rest>
    struct build<symbol::make_sequence, T, type_sequence<First, sequence<Second ...>, Rest ...>>
    {
        using type = type_sequence<sequence<Second ..., First>, Rest ...>;
    };

    // Add previous symbol to current sequence
    template<typename T, typename Second, typename... First, typename... Rest>
    struct build<symbol::make_sequence, T, type_sequence<sequence<First ...>, Second, Rest ...>>
    {
        using type = type_sequence<sequence<Second, First ...>, Rest ...>;
    };

    // Concatenate two sequences
    template<typename T, typename... First, typename... Second, typename... Rest>
    struct build<symbol::make_sequence, T, type_sequence<sequence<First ...>, sequence<Second ...>, Rest ...>>
    {
        using type = type_sequence<sequence<Second ..., First ...>, Rest ...>;
    };

    template<typename T, typename First, typename Second, typename... Rest>
    struct build<symbol::make_alternation, T, type_sequence<First, Second, Rest ...>>
    {
        using type = type_sequence<alternation<Second, First>, Rest ...>;
    };

    template<typename T, typename First, typename... Second, typename... Rest>
    struct build<symbol::make_alternation, T, type_sequence<First, alternation<Second ...>, Rest ...>>
    {
        using type = type_sequence<alternation<Second ..., First>, Rest ...>;
    };

    template<typename T, typename Name, typename First,  typename... Rest>
    struct build<symbol::make_capture<Name>, T, type_sequence<First, Rest ...>>
    {
        static constexpr auto ID = capture_count_v<First, Rest ...> + 1;

        using type = type_sequence<capture<ID, Name, First>, Rest ...>;
    };

    template<std::size_t ID, typename T, typename... Nodes>
    struct build<symbol::make_backref<ID>, T, type_sequence<Nodes ...>>
    {
        using type = type_sequence<backref<ID>, Nodes ...>;
    };

    template<typename Name, typename T, typename... Nodes>
    struct build<symbol::make_named_backref<Name>, T, type_sequence<Nodes ...>>
    {
        using type = type_sequence<named_backref<Name>, Nodes ...>;
    };

    template<match_mode Mode, typename A, typename B, typename T, typename First, typename... Rest>
    struct build<symbol::make_repetition<Mode, A, B>, T, type_sequence<First, Rest ...>>
    {
        using type = type_sequence<basic_repetition<Mode, A, B, First>, Rest ...>;
    };

    // Set building rules
    template<typename T, typename... Nodes>
    struct build<symbol::make_set, T, type_sequence<Nodes ...>>
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
    template<typename T, typename First, typename... Rest>
    struct build<symbol::make_set_from_stack, T, type_sequence<First, nothing, Rest ...>>
    {
        using type = type_sequence<set<First>, Rest ...>;
    };

    template<typename T, typename... First, typename... Rest>
    struct build<symbol::make_set_from_stack, T, type_sequence<set<First ...>, nothing, Rest ...>>
    {
        using type = type_sequence<set<First ...>, Rest ...>;
    };

    template<typename T, typename First, typename... Rest>
    struct build<symbol::make_set_from_stack, T, type_sequence<First, Rest ...>>
    {
        using type = type_sequence<set<First>, Rest ...>;
    };

    template<typename T, typename First, typename... Second, typename... Rest>
    struct build<symbol::make_set_from_stack, T, type_sequence<First, set<Second ...>, Rest ...>>
    {
        using type = type_sequence<set<First, Second ...>, Rest ...>;
    };

    template<typename T, typename... First, typename... Second, typename... Rest>
    struct build<symbol::make_set_from_stack, T, type_sequence<set<First ...>, set<Second ...>, Rest ...>>
    {
        using type = type_sequence<set<First ..., Second ...>, Rest ...>;
    };

    // Create a range inside the set
    template<char B, char A, typename... Second, typename... Rest>
    struct build<symbol::make_range, symbol::character<B>, type_sequence<set<literal<A>, Second ...>, Rest ...>>
    {
        using type = type_sequence<set<range<A, B>, Second ...>, Rest ...>;
    };

    template<char B, typename First, typename... Second, typename... Rest>
    struct build<symbol::make_range, symbol::character<B>, type_sequence<set<First, Second ...>, Rest ...>>
    {
        using type = type_sequence<set<literal<B>, literal<'-'>, First, Second ...>, Rest ...>;
    };

    // Create a range from two last generated AST nodes in the set
    template<typename T, char B, char A, typename... Second, typename... Rest>
    struct build<symbol::make_range_from_stack, T, type_sequence<set<literal<B>, literal<A>, Second ...>, Rest ...>>
    {
        using type = type_sequence<set<range<A, B>, Second ...>, Rest ...>;
    };

    template<typename T, char B, typename First, typename... Second, typename... Rest>
    struct build<symbol::make_range_from_stack, T, type_sequence<set<literal<B>, First, Second ...>, Rest ...>>
    {
        using type = type_sequence<set<literal<B>, literal<'-'>, First, Second ...>, Rest ...>;
    };
}
#endif //MREGEX_AST_BUILDER_HPP