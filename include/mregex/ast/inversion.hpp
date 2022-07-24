#ifndef MREGEX_AST_INVERSION_HPP
#define MREGEX_AST_INVERSION_HPP

#include <mregex/ast/astfwd.hpp>
#include <mregex/utility/type_sequence.hpp>

namespace meta::ast
{
    namespace detail
    {
        template<typename... Ts>
        constexpr auto to_sequence(type_sequence<Ts ...>) noexcept -> sequence<Ts ...>
        {
            return {};
        }

        template<typename... Ts>
        constexpr auto invert(type_sequence<Ts ...>) noexcept
        {
            return (type_sequence<>{}  << ... << Ts{});
        }
    }

    /**
     * Metafuction that inverts (mirrors) the AST at compile-time. This is required to implement
     * lookbehind assertions which need to match a regular expression backwards.
     *
     * @tparam Node The AST node type being inverted
     */
    template<typename Node>
    struct invert
    {
        using type = Node;
    };

    template<typename Node>
    using invert_t = typename invert<Node>::type;

    template<typename... Ts>
    struct invert<sequence<Ts ...>>
    {
        using type = decltype(detail::to_sequence(detail::invert(type_sequence<invert_t<Ts> ...>{})));
    };

    template<template<typename...> typename Wrapper, typename... Inner>
    struct invert<Wrapper<Inner ...>>
    {
        using type = Wrapper<invert_t<Inner> ...>;
    };

    template<match_mode Mode, typename A, typename B, typename Inner>
    struct invert<basic_repetition<Mode, A, B, Inner>>
    {
        using type = basic_repetition<Mode, A, B, invert_t<Inner>>;
    };

    template<std::size_t I, typename Name, typename Inner>
    struct invert<capture<I, Name, Inner>>
    {
        using type = capture<I, Name, invert_t<Inner>>;
    };

    template<typename Inner>
    struct invert<positive_lookahead<Inner>>
    {
        using type = positive_lookbehind<Inner>;
    };

    template<typename Inner>
    struct invert<negative_lookahead<Inner>>
    {
        using type = negative_lookbehind<Inner>;
    };

    template<typename Inner>
    struct invert<positive_lookbehind<Inner>>
    {
        using type = positive_lookahead<Inner>;
    };

    template<typename Inner>
    struct invert<negative_lookbehind<Inner>>
    {
        using type = negative_lookahead<Inner>;
    };
}
#endif //MREGEX_AST_INVERSION_HPP