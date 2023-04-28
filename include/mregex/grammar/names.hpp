#ifndef MREGEX_GRAMMAR_NAMES_HPP
#define MREGEX_GRAMMAR_NAMES_HPP

#include <mregex/grammar/actions.hpp>
#include <mregex/utility/char_traits.hpp>
#include <mregex/utility/type_sequence.hpp>

namespace meta::grammar
{
    /**
     * Metafunction that defines symbols used to begin parsing a name sequence.
     *
     * @tparam Name The metacontainer for the name sequence
     * @tparam C    The current character being parsed
     */
    template<template<char...> typename Name, char C, bool = is_word(C) && !is_digit(C)>
    struct begin_name;

    template<template<char...> typename Name, char C>
    struct begin_name<Name, C, true>
    {
        using type =
                type_sequence
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

    /**
     * Metafunction that defines symbols used to continue parsing a name sequence.
     *
     * @tparam Name The name sequence being parsed
     * @tparam C    The current character being parsed
     */
    template<typename Name, char C, bool = is_word(C)>
    struct continue_name;

    template<template<char...> typename Name, char... Chars, char C>
    struct continue_name<Name<Chars ...>, C, true>
    {
        using type =
                type_sequence
                <
                    advance,
                    Name<Chars ..., C>
                >;
    };

    template<typename Name, char C>
    struct continue_name<Name, C, false>
    {
        using type = reject;
    };

    template<typename Name, char C>
    using continue_name_t = typename continue_name<Name, C>::type;
}
#endif //MREGEX_GRAMMAR_NAMES_HPP