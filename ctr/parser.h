#ifndef CTR_PARSER_H
#define CTR_PARSER_H

#include "static_string.h"
#include "grammar.h"

namespace ctr
{
    /**
     * Generic constexpr LL(1) parser
     *
     * @tparam pattern  The input to be parsed
     */
    template<auto const pattern>
    struct parser
    {
        // helper struct that contains either a symbol::chr or symbol::epsilon based on the side of the index
        template<std::size_t I, bool in_range = I < pattern.length>
        struct character_at
        {
            using type = character<pattern[I]>;
        };

        // specialization for indexes that are out of bound
        template<std::size_t I>
        struct character_at<I, false>
        {
            using type = symbol::epsilon;
        };

        template<std::size_t I>
        using character_at_t = typename character_at<I>::type;

        // forward declare helper structs to resolve reference conflicts
        template<std::size_t I, typename ...>
        struct next_step;

        template<typename Symbol, typename Char, typename AST>
        struct update_ast;

        // main metafunction used to parse the pattern
        template<std::size_t I, typename AST, typename Stack, bool = symbol::is_ast_update_v<typename Stack::top>>
        struct parse
        {
            using next_stack = typename Stack::pop;
            using current_char = character_at_t<I>;
            using current_rule = grammar::rule_t<typename Stack::top, current_char>;

            using type = typename next_step<I, current_rule, AST, next_stack>::type;
        };

        template<std::size_t I, typename AST, typename Stack>
        struct parse<I, AST, Stack, true>
        {
            using next_ast = typename update_ast<typename Stack::top, character_at_t<I - 1>, AST>::type;
            using type = typename parse<I, next_ast, typename Stack::pop>::type;
        };

        // metafunctions to decide the next step in the parsing algorithm
        template<std::size_t I, typename Rule, typename AST, typename Stack>
        struct next_step<I, Rule, AST, Stack>
        {
            using type = typename parse<I, AST, typename Stack::template push<Rule>>::type;
        };

        template<std::size_t I, typename AST, typename Stack>
        struct next_step<I, symbol::epsilon, AST, Stack>
        {
            using type = typename parse<I, AST, Stack>::type;
        };

        template<std::size_t I, typename AST, typename Stack>
        struct next_step<I, grammar::pop_input, AST, Stack>
        {
            using type = typename parse<I + 1, AST, Stack>::type;
        };

        template<std::size_t I, typename AST, typename Stack>
        struct next_step<I, grammar::reject, AST, Stack>
        {
            using type = pair<std::false_type, AST>;
        };

        template<std::size_t I, typename AST, typename Stack>
        struct next_step<I, grammar::accept, AST, Stack>
        {
            using type = pair<std::true_type, AST>;
        };

        // metafunctions for updating the AST
        template<typename C, typename Stack>
        struct update_ast<symbol::make_char, C, Stack>
        {
            using type = typename Stack::template push<C>;
        };

        template<typename C, typename Stack>
        struct update_ast<symbol::make_alnum, C, Stack>
        {
            using type = typename Stack::template push<alnum>;
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

        template<typename C, typename First,  typename ... Rest>
        struct update_ast<symbol::make_capturing, C, stack<First, Rest ...>>
        {
            using type = stack<capturing<First>, Rest ...>;
        };

        using parse_result = typename parse<0, stack<>, stack<symbol::start>>::type;
        using ast_stack = typename parse_result::second;
        using ast = std::conditional_t<std::is_same_v<empty_stack, typename ast_stack::top>,
                epsilon, typename ast_stack::top>;
        static constexpr bool accepted = typename parse_result::first{};
    };
}
#endif //CTR_PARSER_H