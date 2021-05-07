#ifndef CTR_PARSER_H
#define CTR_PARSER_H

#include "static_string.h"
#include "grammar.h"
#include "symbol.h"
#include "stack.h"

namespace ctr
{
    /**
     * Generic constexpr LL(1) parser
     *
     * @tparam Grammar  The grammar used to parse the input
     * @tparam pattern  The input to be parsed
     */
    template<static_string pattern>
    struct parser
    {
        // helper struct that contains either a symbol::character or symbol::epsilon based on the side of the index
        template<std::size_t I, bool in_range = I < pattern.length>
        struct character_at
        {
            using type = symbol::character<pattern[I]>;
        };

        // specialization for indexes that are out of bound
        template<std::size_t I>
        struct character_at<I, false>
        {
            using type = symbol::epsilon;
        };

        template<std::size_t I>
        using character_at_t = typename character_at<I>::type;

        // main method used to parse the pattern
        template<size_t I, typename Stack>
        static constexpr bool parse() noexcept
        {
            using symbol_on_stack = typename Stack::top;
            using next_stack = typename Stack::pop;
            using current_rule = grammar::rule_t<symbol_on_stack, character_at_t<I>>;

            return next_step<I, current_rule, next_stack>::value;
        }

        // helper struct to decide the next step in the parsing algorithm
        template<size_t I, typename Rule, typename Stack>
        struct next_step
        {
            static constexpr auto value = parse<I, typename Stack::template push<Rule>>();
        };

        // if Rule == symbol::epsion, do not advance in the input
        template<size_t I, typename Stack>
        struct next_step<I, symbol::epsilon, Stack>
        {
            static constexpr auto value = parse<I, Stack>();
        };

        // if Rule == grammar::pop_input, advance in the input
        template<size_t I, typename Stack>
        struct next_step<I, grammar::pop_input, Stack>
        {
            static constexpr auto value = parse<I + 1, Stack>();
        };

        // if Rule == grammar::reject, the patterns has a syntactic error
        template<size_t I, typename Stack>
        struct next_step<I, grammar::reject, Stack>
        {
            static constexpr auto value = false;
        };

        // if Rule == grammar::accept, the patterns is accepted
        template<size_t I, typename Stack>
        struct next_step<I, grammar::accept, Stack>
        {
            static constexpr auto value = true;
        };

        static constexpr bool accepted = parse<0, stack<symbol::start>>();
    };
}
#endif //CTR_PARSER_H