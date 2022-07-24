#ifndef MREGEX_NAME_RULES_HPP
#define MREGEX_NAME_RULES_HPP

#include <mregex/grammar/grammar_actions.hpp>
#include <mregex/utility/char_traits.hpp>
#include <mregex/utility/stack.hpp>

namespace meta::grammar
{
    template<template<char...> typename Name, char C, bool = is_word<C> && !is_numeric_v<C>>
    struct begin_name
    {
        using type =
                stack
                <
                    advance,
                    Name<C>
                >;
    };

    template<template<char...> typename Name, char C>
    struct begin_name<Name, C, false>
    {
        using type = reject;
    };

    template<template<char...> typename Name, char C>
    using begin_name_t = typename begin_name<Name, C>::type;

    template<typename, char C, bool = is_word<C>>
    struct update_name
    {
        using type = reject;
    };

    template<template<char...> typename Name, char... Chars, char C>
    struct update_name<Name<Chars ...>, C, true>
    {
        using type =
                stack
                <
                    advance,
                    Name<Chars ..., C>
                >;
    };

    template<typename Name, char C>
    using update_name_t = typename update_name<Name, C>::type;
}
#endif //MREGEX_NAME_RULES_HPP