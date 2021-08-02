#ifndef CX_AST_BUILDER_H
#define CX_AST_BUILDER_H

#include "utility/stack.h"
#include "symbol.h"
#include "ast.h"

namespace cx
{
    /**
     * Metafunction used to build the Abstract Syntax Tree
     *
     * @tparam S    The current symbol on the parsing stack
     * @tparam C    The current character being parsed
     * @tparam AST  The stack containing current AST nodes being built
     */
    template<typename S, typename C, typename AST>
    struct update_ast;

    template<typename C, typename Stack>
    struct update_ast<symbol::make_char, C, Stack>
    {
        using type = typename Stack::template push<C>;
    };

    template<auto A, typename C, typename Stack>
    struct update_ast<symbol::push_char<A>, C, Stack>
    {
        using type = typename Stack::template push<character<A>>;
    };

    template<typename C, typename Stack>
    struct update_ast<symbol::make_newline, C, Stack>
    {
        using type = typename Stack::template push<newline>;
    };

    template<typename C, typename Stack>
    struct update_ast<symbol::make_alpha, C, Stack>
    {
        using type = typename Stack::template push<alpha>;
    };

    template<typename C, typename Stack>
    struct update_ast<symbol::make_digit, C, Stack>
    {
        using type = typename Stack::template push<digit>;
    };

    template<typename C, typename Stack>
    struct update_ast<symbol::make_word, C, Stack>
    {
        using type = typename Stack::template push<word>;
    };

    template<typename C, typename Stack>
    struct update_ast<symbol::make_whitespace, C, Stack>
    {
        using type = typename Stack::template push<whitespace>;
    };

    template<typename C, typename Stack>
    struct update_ast<symbol::make_lower, C, Stack>
    {
        using type = typename Stack::template push<lower>;
    };

    template<typename C, typename Stack>
    struct update_ast<symbol::make_upper, C, Stack>
    {
        using type = typename Stack::template push<upper>;
    };

    template<typename C, typename Stack>
    struct update_ast<symbol::make_hexa, C, Stack>
    {
        using type = typename Stack::template push<hexa>;
    };

    template<typename C, typename Stack>
    struct update_ast<symbol::make_wildcard, C, Stack>
    {
        using type = typename Stack::template push<wildcard>;
    };

    template<typename C, typename Stack>
    struct update_ast<symbol::make_beginning, C, Stack>
    {
        using type = typename Stack::template push<beginning>;
    };

    template<typename C, typename Stack>
    struct update_ast<symbol::make_ending, C, Stack>
    {
        using type = typename Stack::template push<ending>;
    };

    template<typename C, typename First, typename ... Rest>
    struct update_ast<symbol::make_negated, C, stack<First, Rest ...>>
    {
        using type = stack<negated<First>, Rest ...>;
    };

    template<typename C, typename First, typename ... Rest>
    struct update_ast<symbol::make_star, C, stack<First, Rest ...>>
    {
        using type = stack<star<First>, Rest ...>;
    };

    template<typename C, typename First, typename ... Rest>
    struct update_ast<symbol::make_optional, C, stack<First, Rest ...>>
    {
        using type = stack<optional<First>, Rest ...>;
    };

    template<typename C, typename First, typename ... Rest>
    struct update_ast<symbol::make_plus, C, stack<First, Rest ...>>
    {
        using type = stack<plus<First>, Rest ...>;
    };

    // Combine any two non-sequence symbols into a sequence
    template<typename C, typename First, typename Second, typename ... Rest>
    struct update_ast<symbol::make_sequence, C, stack<First, Second, Rest ...>>
    {
        using type = stack<sequence<Second, First>, Rest ...>;
    };

    // Add current symbol to previous sequence
    template<typename C, typename First, typename ... Second, typename ... Rest>
    struct update_ast<symbol::make_sequence, C, stack<First, sequence<Second ...>, Rest ...>>
    {
        using type = stack<sequence<Second ..., First>, Rest ...>;
    };

    // Add previous symbol to current sequence
    template<typename C, typename Second, typename ... First, typename ... Rest>
    struct update_ast<symbol::make_sequence, C, stack<sequence<First ...>, Second, Rest ...>>
    {
        using type = stack<sequence<Second, First ...>, Rest ...>;
    };

    // Concatenate two sequences
    template<typename C, typename ... First, typename ... Second, typename ... Rest>
    struct update_ast<symbol::make_sequence, C, stack<sequence<First ...>, sequence<Second ...>, Rest ...>>
    {
        using type = stack<sequence<Second ..., First ...>, Rest ...>;
    };

    template<typename C, typename First, typename Second, typename ... Rest>
    struct update_ast<symbol::make_alternation, C, stack<First, Second, Rest ...>>
    {
        using type = stack<alternation<Second, First>, Rest ...>;
    };

    template<typename C, typename First, typename ... Second, typename ... Rest>
    struct update_ast<symbol::make_alternation, C, stack<First, alternation<Second ...>, Rest ...>>
    {
        using type = stack<alternation<Second ..., First>, Rest ...>;
    };

    template<typename C, typename ... Elems>
    struct update_ast<symbol::make_set, C, stack<Elems ...>>
    {
        // Push a null node to prevent set alternation combining with previous alternations
        using type = stack<null, Elems ...>;
    };

    // Temporarily wrap AST node into symbol::captureless_wrapper to avoid making a capturing group later
    template<typename C, typename First,  typename ... Rest>
    struct update_ast<symbol::make_captureless, C, stack<First, Rest ...>>
    {
        using type = stack<symbol::captureless_wrapper<First>, Rest ...>;
    };

    template<typename C, typename First,  typename ... Rest>
    struct update_ast<symbol::make_atomic, C, stack<First, Rest ...>>
    {
    using type = stack<atomic<First>, Rest ...>;
    };

    template<typename C, typename First,  typename ... Rest>
    struct update_ast<symbol::make_capturing, C, stack<First, Rest ...>>
    {
        static constexpr auto ID = capture_counter<First, Rest ...>::count + 1;
        using type = stack<capturing<ID, First>, Rest ...>;
    };

    // Captureless mode has higher priority than the default capturing mode for groups
    template<typename C, typename First,  typename ... Rest>
    struct update_ast<symbol::make_capturing, C, stack<symbol::captureless_wrapper<First>, Rest ...>>
    {
            using type = stack<First, Rest ...>;
    };

    template<std::size_t ID, typename C, typename ... Elems>
    struct update_ast<symbol::make_backref<ID>, C, stack<Elems ...>>
    {
        using type = stack<backref<ID>, Elems ...>;
    };

    template<std::size_t N, typename C, typename First, typename ... Rest>
    struct update_ast<symbol::make_repeated<N>, C, stack<First, Rest ...>>
    {
        using type = stack<repeated<N, First>, Rest ...>;
    };

    template<typename C, typename ... First>
    struct update_ast<symbol::make_set_from_current_char, C, stack<First ...>>
    {
        using type = stack<alternation<C>, First ...>;
    };

    template<typename C, typename ... First, typename ... Rest>
    struct update_ast<symbol::make_set_from_current_char, C, stack<alternation<First ...>, Rest ...>>
    {
        using type = stack<alternation<C, First ...>, Rest ...>;
    };

    template<typename C, typename ... Rest>
    struct update_ast<symbol::make_set_from_current_char, C, stack<null, Rest ...>>
    {
        using type = stack<alternation<C>, Rest ...>;
    };

    template<typename C, typename First, typename ... Rest>
    struct update_ast<symbol::make_set_from_stack, C, stack<First, Rest ...>>
    {
        using type = stack<alternation<First>, Rest ...>;
    };

    template<typename C, typename First, typename ... Second, typename ... Rest>
    struct update_ast<symbol::make_set_from_stack, C, stack<First, alternation<Second ...>, Rest ...>>
    {
        using type = stack<alternation<First, Second ...>, Rest ...>;
    };

    template<typename C, typename First, typename ... Rest>
    struct update_ast<symbol::make_set_from_stack, C, stack<First, null, Rest ...>>
    {
        using type = stack<alternation<First>, Rest ...>;
    };

    template<typename C, typename ... First, typename ... Second, typename ... Rest>
    struct update_ast<symbol::make_set_from_stack, C, stack<alternation<First ...>, alternation<Second ...>, Rest ...>>
    {
        using type = stack<alternation<First ..., Second ...>, Rest ...>;
    };

    template<auto B, auto A, typename ... Second, typename ... Rest>
    struct update_ast<symbol::make_range, character<B>, stack<alternation<character<A>, Second ...>, Rest ...>>
    {
        using type = stack<alternation<range<A, B>, Second ...>, Rest ...>;
    };

    template<typename C, auto B, auto A, typename ... Second, typename ... Rest>
    struct update_ast<symbol::make_range_from_stack, C, stack<alternation<character<B>, character<A>, Second ...>, Rest ...>>
    {
        using type = stack<alternation<range<A, B>, Second ...>, Rest ...>;
    };
}
#endif //CX_AST_BUILDER_H