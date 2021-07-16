#ifndef CX_AST_BUILDER_H
#define CX_AST_BUILDER_H

#include "utility/stack.h"
#include "symbol.h"
#include "ast.h"

namespace cx
{
    /**
     * Declaration fo the update_ast metafunction
     *
     * @tparam  The current symbol on the stack
     * @tparam  The current character in the parsed input
     * @tparam  The current active of the stack with the AST nodes
     */
    template<typename, typename, typename>
    struct update_ast;

    template<typename C, typename Stack>
    struct update_ast<symbol::make_char, C, Stack>
    {
        using type = typename Stack::template push<C>;
    };

    template<typename C, typename Stack>
    struct update_ast<symbol::make_alnum, C, Stack>
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

    template<typename C, typename First, typename Second, typename ... Rest>
    struct update_ast<symbol::make_sequence, C, stack<First, Second, Rest ...>>
    {
        using type = stack<sequence<Second, First>, Rest ...>;
    };

    template<typename C, typename First, typename ... Second, typename ... Rest>
    struct update_ast<symbol::make_sequence, C, stack<First, sequence<Second ...>, Rest ...>>
    {
        using type = stack<sequence<Second ..., First>, Rest ...>;
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
        static constexpr auto ID = count_captures<First, Rest ...>::capture_count + 1;
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
