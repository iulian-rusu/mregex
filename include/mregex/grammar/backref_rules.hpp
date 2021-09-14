#ifndef MREGEX_BACKREF_RULES_HPP
#define MREGEX_BACKREF_RULES_HPP

#include <mregex/symbol.hpp>
#include <mregex/utility/stack.hpp>
#include <mregex/utility/char_traits.hpp>
#include <mregex/grammar/grammar_actions.hpp>

namespace meta::grammar
{
    /**
     * Metafunction that decides if the current rule will continue
     * the backreference ID building or will finish it.
     *
     * @tparam C    The current character in the input pattern
     * @tparam ID   The current backreference ID on the stack
     */
    template<auto C, std::size_t ID, bool = is_numeric_v<C>>
    struct backref_rule
    {
        using type =
                stack
                <
                    advance,
                    symbol::backref_id<10 * ID + C - '0'>
                >;
    };

    template<auto C, std::size_t ID>
    struct backref_rule<C, ID, false>
    {
        using type = symbol::make_backref<ID>;
    };

    template<auto C, std::size_t ID>
    using backref_rule_t = typename backref_rule<C, ID>::type;
}
#endif //MREGEX_BACKREF_RULES_HPP