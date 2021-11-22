#ifndef MREGEX_AST_BUILDER_HPP
#define MREGEX_AST_BUILDER_HPP

#include <mregex/symbol.hpp>
#include <mregex/utility/stack.hpp>
#include <mregex/ast/astfwd.hpp>
#include <mregex/ast/ast_traits.hpp>

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

    template<auto C, typename Stack>
    struct build<symbol::make_literal, symbol::character<C>, Stack>
    {
        using type = push<Stack, literal<C>>;
    };

    template<auto A, typename T, typename Stack>
    struct build<symbol::push_literal<A>, T, Stack>
    {
        using type = push<Stack, literal<A>>;
    };

    template<typename T, typename Stack>
    struct build<symbol::make_alpha, T, Stack>
    {
        using type = push<Stack, alpha>;
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
    struct build<symbol::make_ending, T, Stack>
    {
        using type = push<Stack, ending>;
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
    struct build<symbol::make_negated, T, stack<First, Rest ...>>
    {
        using type = stack<negated<First>, Rest ...>;
    };

    template<typename T, typename First, typename... Rest>
    struct build<symbol::make_star, T, stack<First, Rest ...>>
    {
        using type = stack<star<First>, Rest ...>;
    };

    template<typename T, typename First, typename... Rest>
    struct build<symbol::make_optional, T, stack<First, Rest ...>>
    {
        using type = stack<optional<First>, Rest ...>;
    };

    template<typename T, typename First, typename... Rest>
    struct build<symbol::make_plus, T, stack<First, Rest ...>>
    {
        using type = stack<plus<First>, Rest ...>;
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

    template<typename T, typename First,  typename... Rest>
    struct build<symbol::make_capturing, T, stack<First, Rest ...>>
    {
        static constexpr auto ID = capture_count_v<First, Rest ...> + 1;

        using type = stack<capturing<ID, First>, Rest ...>;
    };

    template<std::size_t ID, typename T, typename... Elems>
    struct build<symbol::make_backref<ID>, T, stack<Elems ...>>
    {
        using type = stack<backref<ID>, Elems ...>;
    };

    template<typename A, typename B, typename T, typename First, typename... Rest>
    struct build<symbol::make_repetition<A, B>, T, stack<First, Rest ...>>
    {
        using type = stack<repetition<A, B, First>, Rest ...>;
    };

    // Set building rules
    template<typename T, typename... Elems>
    struct build<symbol::make_set, T, stack<Elems ...>>
    {
        using type = stack<nothing, Elems ...>;
    };

    template<auto C, typename... Elems>
    struct build<symbol::make_set_from_current_char, symbol::character<C>, stack<Elems ...>>
    {
        using type = stack<set<literal<C>>, Elems ...>;
    };

    template<auto C, typename... First, typename... Rest>
    struct build<symbol::make_set_from_current_char, symbol::character<C>, stack<set<First ...>, Rest ...>>
    {
        using type = stack<set<literal<C>, First ...>, Rest ...>;
    };

    template<auto C, typename... Rest>
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

    template<auto B, auto A, typename... Second, typename... Rest>
    struct build<symbol::make_range, symbol::character<B>, stack<set<literal<A>, Second ...>, Rest ...>>
    {
        using type = stack<set<range<A, B>, Second ...>, Rest ...>;
    };

    template<typename T, auto B, auto A, typename... Second, typename... Rest>
    struct build<symbol::make_range_from_stack, T, stack<set<literal<B>, literal<A>, Second ...>, Rest ...>>
    {
        using type = stack<set<range<A, B>, Second ...>, Rest ...>;
    };
}
#endif //MREGEX_AST_BUILDER_HPP