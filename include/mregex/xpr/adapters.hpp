#ifndef MREGEX_XPR_ADAPTERS_HPP
#define MREGEX_XPR_ADAPTERS_HPP

#include <mregex/ast/astfwd.hpp>
#include <mregex/regex_interface.hpp>

namespace meta::xpr
{
    namespace detail
    {
        template<static_string String, std::size_t... Indices>
        constexpr auto to_sequence(std::index_sequence<Indices ...>) noexcept
        {
            return ast::sequence<ast::literal<String[Indices]> ...>{};
        }

        template<typename Wrapper>
        constexpr auto flatten(Wrapper) noexcept -> Wrapper { return {}; }

        template<template<typename...> typename Wrapper, typename First, typename... Second>
        constexpr auto flatten(Wrapper<First, Wrapper<Second ...>>) noexcept -> Wrapper<First, Second ...> { return {}; }

        template<template<typename...> typename Wrapper, typename... First, typename Second>
        constexpr auto flatten(Wrapper<Wrapper<First ...>, Second>) noexcept -> Wrapper<First ..., Second> { return {}; }

        template<template<typename...> typename Wrapper, typename... First, typename... Second>
        constexpr auto flatten(Wrapper<Wrapper<First ...>, Wrapper<Second ...>>) noexcept -> Wrapper<First ..., Second ...> { return {}; }
    }

    /**
     * Converts a static string into an instance of ast::sequence.
     *
     * @tparam String   The static string to be converted
     * @return          An instance of ast::sequence equivalent to the original string
     */
    template<static_string String>
    constexpr auto to_sequence() noexcept
    {
        return detail::to_sequence<String>(std::make_index_sequence<String.length()>{});
    }

    /**
     * Converts an Abstract Syntax Tree to a type which provides a regex-like interface.
     *
     * @tparam AST  The Abstract Syntax Tree of the regex
     * @return      An object with a regex-like interface
     */
    template<typename AST>
    constexpr auto to_regex(AST) noexcept -> regex_interface<AST> { return {}; }

    /**
     * Packs a sequence of elements into a wrapper that holds a single type.
     * If the list contains more than one type, it is wrapped into an ast::sequence.
     *
     * @tparam Wrapper  The type of the wrapper
     * @tparam Nodes    The list of elements to be wrapped
     * @return          An instance of the wrapper type
     */
    template<template<typename> typename Wrapper, typename... Nodes>
    constexpr auto pack_sequence(Nodes...) -> Wrapper<ast::sequence<Nodes ...>> { return {}; }

    template<template<typename> typename Wrapper, typename Node>
    constexpr auto pack_sequence(Node) -> Wrapper<Node> { return {}; }

    /**
     * Wraps a sequence of elements into a wrapper type and flattens the result.
     * Flattening will remove any redundant wrappers from the resulting type.
     *
     * @tparam Wrapper  The type of the wrapper
     * @tparam First    The first element in the sequence
     * @tparam Second   The second element in the sequence
     * @tparam Rest     The rest of the sequence
     * @return          A flattened instance of the wrapper type
     */
    template<template<typename...> typename Wrapper, typename First, typename Second, typename... Rest>
    constexpr auto flat_wrap_sequence(First, Second, Rest... rest) noexcept
    {
        constexpr auto wrapped = detail::flatten(Wrapper<First, Second>{});
        if constexpr(sizeof...(Rest) == 0)
            return wrapped;
        else
            return flat_wrap_sequence<Wrapper>(wrapped, rest ...);
    }

    template<template<typename...> typename Wrapper, typename Node>
    constexpr auto flat_wrap_sequence(Node) noexcept -> Node { return {}; }

    template<template<typename...> typename Wrapper>
    constexpr auto flat_wrap_sequence() noexcept -> Wrapper<> { return {}; }
}
#endif //MREGEX_XPR_ADAPTERS_HPP