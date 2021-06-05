#ifndef CX_PARSER_H
#define CX_PARSER_H

#include "static_string.h"
#include "grammar.h"
#include "ast_builder.h"
#include "capture_builder.h"
#include "type_helpers.h"

namespace cx
{
    /**
     * Constexpr parser using the LL(1) algorithm
     *
     * @tparam pattern  The input to be parsed
     */
    template<auto const pattern>
    struct parser
    {
        // helper struct that contains either a symbol::character or symbol::epsilon based on the size of the index
        template<std::size_t I, bool = I < pattern.length()>
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

        // forward declare helper struct to resolve reference conflicts
        template<std::size_t I, typename ...>
        struct next_step;

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

        using parse_result = typename parse<0, stack<>, stack<symbol::start>>::type;
        using stack_top = typename parse_result::second::top;
        using ast = std::conditional_t<std::is_same_v<empty_stack, stack_top>, epsilon, renumber_captures_t<0, stack_top>>;
        static constexpr bool accepted = typename parse_result::first{};
    };
}
#endif //CX_PARSER_H