#ifndef MREGEX_AST_INVERSION_HPP
#define MREGEX_AST_INVERSION_HPP

#include <mregex/ast/astfwd.hpp>

namespace meta::ast
{
    namespace detail
    {
        template<typename... Ts>
        constexpr auto to_sequence(stack<Ts ...>) noexcept -> sequence<Ts ...>
        {
            return {};
        }

        template<typename T, typename... Ts>
        constexpr auto operator+(stack<Ts ...>, T) noexcept -> stack<T, Ts ...>
        {
            return {};
        }

        template<typename... Ts>
        constexpr auto invert_stack(stack<Ts ...>) noexcept
        {
            return (stack<>{}  + ... + Ts{});
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
        using type = decltype(detail::to_sequence(detail::invert_stack(stack<invert_t<Ts> ...>{})));
    };

    template<template<typename...> typename Wrapper, typename... Inner>
    struct invert<Wrapper<Inner ...>>
    {
        using type = Wrapper<invert_t<Inner> ...>;
    };

    template<typename A, typename B, typename Inner>
    struct invert<repetition<A, B, Inner>>
    {
        using type = repetition<A, B, invert_t<Inner>>;
    };

    template<auto I, typename Inner>
    struct invert<capture<I, Inner>>
    {
        using type = capture<I, invert_t<Inner>>;
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