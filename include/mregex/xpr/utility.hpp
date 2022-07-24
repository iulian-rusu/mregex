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
     * Metafunction that wraps a pack of types into a metacontainer and flattens
     * the result if it containts a single type.
     *
     * @tparam Wrapper  The type of the wrapper
     * @tparam Nodes    The variadic parameter pack of elements to be wrapped
     */
    template<template<typename...> typename Wrapper, typename... Nodes>
    struct flatten_wrapper
    {
        using type = Wrapper<Nodes ...>;
    };

    template<template<typename...> typename Wrapper, typename Node>
    struct flatten_wrapper<Wrapper, Node>
    {
        using type = Node;
    };

    template<template<typename...> typename Wrapper, typename... Nodes>
    using flatten_wrapper_t = typename flatten_wrapper<Wrapper, Nodes ...>::type;

    /**
     * Wraps a pack of elements into a sequence and passes them to a wrapper template.
     *
     * @tparam Wrapper  The type of the wrapper
     * @tparam Nodes    The variadic parameter pack of elements to be wrapped
     * @return          An object of the wrapped type
     */
    template<template<typename> typename Wrapper, typename... Nodes>
    constexpr auto wrap(Nodes...) -> Wrapper<flatten_wrapper_t<ast::sequence, Nodes ...>> { return {}; }

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
}
#endif //MREGEX_XPR_UTILITY_HPP