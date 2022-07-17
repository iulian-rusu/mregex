#ifndef MREGEX_BACKREF_RULES_HPP
#define MREGEX_BACKREF_RULES_HPP

#include <mregex/grammar/grammar_actions.hpp>
#include <mregex/utility/char_traits.hpp>
#include <mregex/utility/stack.hpp>
#include <mregex/symbols.hpp>

namespace meta::grammar
{
    /**
     * Metafunction that decides if the current rule will continue
     * the backreference ID building or will finish it.
     *
     * @tparam ID   The current backreference ID on the stack
     * @tparam C    The current character in the input pattern
     */
    template<std::size_t ID, char C, bool = is_numeric_v<C>>
    struct backref_rule
    {
        using type =
                stack
                <
                    advance,
                    symbol::backref_id<10 * ID + C - '0'>
                >;
    };

    template<std::size_t ID, char C>
    struct backref_rule<ID, C, false>
    {
        using type = symbol::make_backref<ID>;
    };

    template<std::size_t ID, char C>
    using backref_rule_t = typename backref_rule<ID, C>::type;
}
#endif //MREGEX_BACKREF_RULES_HPP