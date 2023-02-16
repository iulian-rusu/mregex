#ifndef MREGEX_GRAMMAR_NAMES_HPP
#define MREGEX_GRAMMAR_NAMES_HPP

#include <mregex/grammar/actions.hpp>
#include <mregex/utility/char_traits.hpp>
#include <mregex/utility/type_sequence.hpp>

namespace meta::grammar
{
    /**
     * Metafunction that defines symbols used to parse name sequences.
     * Name sequences are used to identify named captures and named backreferences.
     */
    template<template<char...> typename Name, char C, bool = is_word(C) && !is_numeric(C)>
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
     * Metafunction that defines symbols used to update a name sequence.
     * This update can add more characters or finish parsing the name sequence.
     */
    template<typename Name, char C, bool = is_word(C)>
    struct update_name;

    template<template<char...> typename Name, char... Chars, char C>
    struct update_name<Name<Chars ...>, C, true>
    {
        using type =
                type_sequence
                <
                    advance,
                    Name<Chars ..., C>
                >;
    };

    template<typename Name, char C>
    struct update_name<Name, C, false>
    {
        using type = reject;
    };

    template<typename Name, char C>
    using update_name_t = typename update_name<Name, C>::type;
}
#endif //MREGEX_GRAMMAR_NAMES_HPP