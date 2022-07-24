#ifndef MREGEX_XPR_UTILITY_HPP
#define MREGEX_XPR_UTILITY_HPP

#include <mregex/ast/astfwd.hpp>
#include <mregex/regex_interface.hpp>

namespace meta::xpr
{
    namespace detail
    {
        template<static_string Str, std::size_t... Indices>
        constexpr auto to_sequence(std::index_sequence<Indices ...>) noexcept
        {
            return ast::sequence<ast::literal<Str[Indices]> ...>{};
        }
    }

    /**
     * Transforms a static string into an instance of ast::sequence.
     *
     * @tparam Str  The static string to be transform
     * @return      An instance of ast::sequence equivalent to the original string
     */
    template<static_string Str>
    constexpr auto to_sequence() noexcept
    {
        return detail::to_sequence<Str>(std::make_index_sequence<Str.length()>{});
    }

    /**
     * Adapts an Abstract Syntax Tree to give it the same interface as
     * a standard regex type.
     *
     * @tparam AST  The Abstract Syntax Tree type
     * @return      An object with the interface of a regular expression
     */
    template<typename AST>
    constexpr auto to_regex(AST) noexcept -> regex_interface<AST> { return {}; }

    /**
     * Packs a list of elements into a wrapper that holds a single type.
     * If the list contains more than one type, it is wrapped into a sequence.
     *
     * @tparam Wrapper  The type of the wrapper
     * @tparam Nodes    The list of elements to be wrapped
     * @return          An instance of the wrapper type
     */
    template<template<typename> typename Wrapper, typename Node>
    constexpr auto pack(Node) -> Wrapper<Node> { return {}; }

    template<template<typename> typename Wrapper, typename... Nodes>
    constexpr auto pack(Nodes...) -> Wrapper<ast::sequence<Nodes ...>> { return {}; }

    /**
     * Function that combines multiple AST nodes into a wrapper metacontainer
     * and flattens the resulting structure to obtain a single nesting level.
     */
    template<template<typename...> typename Wrapper, typename... Nodes>
    constexpr auto flat_wrap(Nodes...) noexcept -> Wrapper<Nodes ...> { return {}; }

    template<template<typename...> typename, typename Node>
    constexpr auto flat_wrap(Node) noexcept -> Node { return {}; }

    template<template<typename...> typename Wrapper, typename First, typename... Second>
    constexpr auto flat_wrap(First, Wrapper<Second ...>) noexcept -> Wrapper<First, Second ...> { return {}; }

    template<template<typename...> typename Wrapper, typename... First, typename Second>
    constexpr auto flat_wrap(Wrapper<First ...>, Second) noexcept -> Wrapper<First ..., Second> { return {}; }

    template<template<typename...> typename Wrapper, typename... First, typename... Second>
    constexpr auto flat_wrap(Wrapper<First ...>, Wrapper<Second ...>) noexcept -> Wrapper<First ..., Second ...> { return {}; }
}
#endif //MREGEX_XPR_UTILITY_HPP