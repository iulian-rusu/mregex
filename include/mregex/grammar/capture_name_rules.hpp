#ifndef MREGEX_CAPTURE_NAME_RULES_HPP
#define MREGEX_CAPTURE_NAME_RULES_HPP

#include <mregex/grammar/grammar_actions.hpp>
#include <mregex/utility/char_traits.hpp>
#include <mregex/utility/stack.hpp>
#include <mregex/symbol.hpp>

namespace meta::grammar
{
    template<char C, bool = is_word<C> && !is_numeric_v<C>>
    struct begin_capture_name
    {
        using type =
                stack
                <
                    advance,
                    symbol::capture_name_seq<C>
                >;
    };

    template<char C>
    struct begin_capture_name<C, false>
    {
        using type = reject;
    };

    template<char C>
    using begin_capture_name_t = typename begin_capture_name<C>::type;

    template<typename Name, char C, bool = is_word<C>>
    struct update_capture_name
    {
        using type = reject;
    };

    template<char... Chars, char C>
    struct update_capture_name<symbol::capture_name_seq<Chars ...>, C, true>
    {
        using type =
                stack
                <
                    advance,
                    symbol::capture_name_seq<Chars ..., C>
                >;
    };

    template<typename Name, char C>
    using update_capture_name_t = typename update_capture_name<Name, C>::type;
}
#endif //MREGEX_CAPTURE_NAME_RULES_HPP
