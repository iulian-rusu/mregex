#ifndef MREGEX_AST_BUILDER_HPP
#define MREGEX_AST_BUILDER_HPP

#include <mregex/ast/astfwd.hpp>
#include <mregex/ast/traits.hpp>
#include <mregex/utility/stack.hpp>

namespace meta::ast
{
    /**
     * Metafunction used to build the Abstract Syntax Tree.
     *
     * @tparam Symbol   The current symbol on the parsing stack
     * @tparam Token    The current token being parsed
     * @tparam AST      The stack with the current state of the AST
     */
    template<typename Symbol, typename Token, typename AST>
    struct build;

    template<typename Symbol, typename Token, typename AST>
    using build_t = typename build<Symbol, Token, AST>::type;

    template<char C, typename Stack>
    struct build<symbol::make_literal, symbol::character<C>, Stack>
    {
        using type = push<Stack, literal<C>>;
    };

    template<char A, typename T, typename Stack>
    struct build<symbol::push_literal<A>, T, Stack>
    {
        using type = push<Stack, literal<A>>;
    };

    template<typename T, typename Stack>
    struct build<symbol::make_empty, T, Stack>
    {
        using type = push<Stack, empty>;
    };

    template<typename T, typename Stack>
    struct build<symbol::make_digit, T, Stack>
    {
        using type = push<Stack, digit>;
    };

    template<typename T, typename Stack>
    struct build<symbol::make_word, T, Stack>
    {
        using type = push<Stack, word>;
    };

    template<typename T, typename Stack>
    struct build<symbol::make_word_boundary, T, Stack>
    {
        using type = push<Stack, word_boundary>;
    };

    template<typename T, typename Stack>
    struct build<symbol::make_whitespace, T, Stack>
    {
        using type = push<Stack, whitespace>;
    };

    template<typename T, typename Stack>
    struct build<symbol::make_lower, T, Stack>
    {
        using type = push<Stack, lower>;
    };

    template<typename T, typename Stack>
    struct build<symbol::make_upper, T, Stack>
    {
        using type = push<Stack, upper>;
    };

    template<typename T, typename Stack>
    struct build<symbol::make_hexa, T, Stack>
    {
        using type = push<Stack, hexa>;
    };

    template<typename T, typename Stack>
    struct build<symbol::make_linebreak, T, Stack>
    {
        using type = push<Stack, linebreak>;
    };

    template<typename T, typename Stack>
    struct build<symbol::make_wildcard, T, Stack>
    {
        using type = push<Stack, wildcard>;
    };

    template<typename T, typename Stack>
    struct build<symbol::make_beginning, T, Stack>
    {
        using type = push<Stack, beginning>;
    };

    template<typename T, typename Stack>
    struct build<symbol::make_end, T, Stack>
    {
        using type = push<Stack, end>;
    };

    template<typename T, typename Stack>
    struct build<symbol::make_beginning_of_input, T, Stack>
    {
        using type = push<Stack, beginning_of_input>;
    };

    template<typename T, typename Stack>
    struct build<symbol::make_end_of_input, T, Stack>
    {
        using type = push<Stack, end_of_input>;
    };

    template<typename T, typename First, typename... Rest>
    struct build<symbol::make_positive_lookahead, T, stack<First, Rest ...>>
    {
        using type = stack<positive_lookahead<First>, Rest ...>;
    };

    template<typename T, typename First, typename... Rest>
    struct build<symbol::make_negative_lookahead, T, stack<First, Rest ...>>
    {
        using type = stack<negative_lookahead<First>, Rest ...>;
    };

    template<typename T, typename First, typename... Rest>
    struct build<symbol::make_positive_lookbehind, T, stack<First, Rest ...>>
    {
        using type = stack<positive_lookbehind<First>, Rest ...>;
    };

    template<typename T, typename First, typename... Rest>
    struct build<symbol::make_negative_lookbehind, T, stack<First, Rest ...>>
    {
        using type = stack<negative_lookbehind<First>, Rest ...>;
    };

    template<typename T, typename First, typename... Rest>
    struct build<symbol::make_negated, T, stack<First, Rest ...>>
    {
        using type = stack<negated<First>, Rest ...>;
    };

    template<match_mode Mode, typename T, typename First, typename... Rest>
    struct build<symbol::make_star<Mode>, T, stack<First, Rest ...>>
    {
        using type = stack<basic_star<Mode, First>, Rest ...>;
    };

    template<match_mode Mode, typename T, typename First, typename... Rest>
    struct build<symbol::make_optional<Mode>, T, stack<First, Rest ...>>
    {
        using type = stack<basic_optional<Mode, First>, Rest ...>;
    };

    template<match_mode Mode, typename T, typename First, typename... Rest>
    struct build<symbol::make_plus<Mode>, T, stack<First, Rest ...>>
    {
        using type = stack<basic_plus<Mode, First>, Rest ...>;
    };


    // Combine any two non-sequence symbols into a sequence
    template<typename T, typename First, typename Second, typename... Rest>
    struct build<symbol::make_sequence, T, stack<First, Second, Rest ...>>
    {
        using type = stack<sequence<Second, First>, Rest ...>;
    };

    // Add current symbol to previous sequence
    template<typename T, typename First, typename... Second, typename... Rest>
    struct build<symbol::make_sequence, T, stack<First, sequence<Second ...>, Rest ...>>
    {
        using type = stack<sequence<Second ..., First>, Rest ...>;
    };

    // Add previous symbol to current sequence
    template<typename T, typename Second, typename... First, typename... Rest>
    struct build<symbol::make_sequence, T, stack<sequence<First ...>, Second, Rest ...>>
    {
        using type = stack<sequence<Second, First ...>, Rest ...>;
    };

    // Concatenate two sequences
    template<typename T, typename... First, typename... Second, typename... Rest>
    struct build<symbol::make_sequence, T, stack<sequence<First ...>, sequence<Second ...>, Rest ...>>
    {
        using type = stack<sequence<Second ..., First ...>, Rest ...>;
    };

    template<typename T, typename First, typename Second, typename... Rest>
    struct build<symbol::make_alternation, T, stack<First, Second, Rest ...>>
    {
        using type = stack<alternation<Second, First>, Rest ...>;
    };

    template<typename T, typename First, typename... Second, typename... Rest>
    struct build<symbol::make_alternation, T, stack<First, alternation<Second ...>, Rest ...>>
    {
        using type = stack<alternation<Second ..., First>, Rest ...>;
    };

    template<typename T, typename Name, typename First,  typename... Rest>
    struct build<symbol::make_capture<Name>, T, stack<First, Rest ...>>
    {
        static constexpr auto ID = capture_count_v<First, Rest ...> + 1;

        using type = stack<capture<ID, Name, First>, Rest ...>;
    };

    template<std::size_t ID, typename T, typename... Nodes>
    struct build<symbol::make_backref<ID>, T, stack<Nodes ...>>
    {
        using type = stack<backref<ID>, Nodes ...>;
    };

    template<typename Name, typename T, typename... Nodes>
    struct build<symbol::make_named_backref<Name>, T, stack<Nodes ...>>
    {
        using type = stack<named_backref<Name>, Nodes ...>;
    };

    template<match_mode Mode, typename A, typename B, typename T, typename First, typename... Rest>
    struct build<symbol::make_repetition<Mode, A, B>, T, stack<First, Rest ...>>
    {
        using type = stack<basic_repetition<Mode, A, B, First>, Rest ...>;
    };

    // Set building rules
    template<typename T, typename... Nodes>
    struct build<symbol::make_set, T, stack<Nodes ...>>
    {
        using type = stack<nothing, Nodes ...>;
    };

    template<char C, typename... Nodes>
    struct build<symbol::make_set_from_current_char, symbol::character<C>, stack<Nodes ...>>
    {
        using type = stack<set<literal<C>>, Nodes ...>;
    };

    template<char C, typename... First, typename... Rest>
    struct build<symbol::make_set_from_current_char, symbol::character<C>, stack<set<First ...>, Rest ...>>
    {
        using type = stack<set<literal<C>, First ...>, Rest ...>;
    };

    template<char C, typename... Rest>
    struct build<symbol::make_set_from_current_char, symbol::character<C>, stack<nothing, Rest ...>>
    {
        using type = stack<set<literal<C>>, Rest ...>;
    };

    template<typename T, typename First, typename... Rest>
    struct build<symbol::make_set_from_stack, T, stack<First, Rest ...>>
    {
        using type = stack<set<First>, Rest ...>;
    };

    template<typename T, typename First, typename... Second, typename... Rest>
    struct build<symbol::make_set_from_stack, T, stack<First, set<Second ...>, Rest ...>>
    {
        using type = stack<set<First, Second ...>, Rest ...>;
    };

    template<typename T, typename First, typename... Rest>
    struct build<symbol::make_set_from_stack, T, stack<First, nothing, Rest ...>>
    {
        using type = stack<set<First>, Rest ...>;
    };

    template<typename T, typename... First, typename... Second, typename... Rest>
    struct build<symbol::make_set_from_stack, T, stack<set<First ...>, set<Second ...>, Rest ...>>
    {
        using type = stack<set<First ..., Second ...>, Rest ...>;
    };

    template<char B, char A, typename... Second, typename... Rest>
    struct build<symbol::make_range, symbol::character<B>, stack<set<literal<A>, Second ...>, Rest ...>>
    {
        using type = stack<set<range<A, B>, Second ...>, Rest ...>;
    };

    template<char B, typename First, typename... Second, typename... Rest>
    struct build<symbol::make_range, symbol::character<B>, stack<set<First, Second ...>, Rest ...>>
    {
        using type = stack<set<literal<B>, literal<'-'>, First, Second ...>, Rest ...>;
    };

    template<typename T, char B, char A, typename... Second, typename... Rest>
    struct build<symbol::make_range_from_stack, T, stack<set<literal<B>, literal<A>, Second ...>, Rest ...>>
    {
        using type = stack<set<range<A, B>, Second ...>, Rest ...>;
    };

    template<typename T, char B, typename First, typename... Second, typename... Rest>
    struct build<symbol::make_range_from_stack, T, stack<set<literal<B>, First, Second ...>, Rest ...>>
    {
        using type = stack<set<literal<B>, literal<'-'>, First, Second ...>, Rest ...>;
    };
}
#endif //MREGEX_AST_BUILDER_HPP