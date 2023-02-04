#ifndef MREGEX_AST_TRAITS_HPP
#define MREGEX_AST_TRAITS_HPP

#include <string_view>
#include <mregex/ast/astfwd.hpp>
#include <mregex/utility/type_sequence.hpp>

namespace meta::ast
{
    /**
     * Type trait to identify nodes that can be trivially matched.
     * A type T is trivially matchable if it always consumes one character when matching.
     * A type T is detected as trivially matchable by checking if it contains a static member function
     * template match_one<A, B>, where A satisfies std::forward_iterator and B is any generic type.
     */
    template<typename Node>
    inline constexpr bool is_trivially_matchable = requires { &Node::template match_one<const char *, int>; };

    template<typename... Nodes>
    inline constexpr bool are_trivially_matchable = (is_trivially_matchable<Nodes> && ...);

    /**
     * Metafunction used to count the number of capturing groups in the regex AST.
     *
     * @tparam First    The first AST node
     * @tparam Rest     The rest of AST nodes
     */
    template<typename First, typename... Rest>
    inline constexpr std::size_t count_captures = First::capture_count + (Rest::capture_count + ... + 0);

    /**
    * Metafunction that builds a specification for the names of all capturing groups in the AST.
    * The specification lists all capture names according to Preorder AST traversal.
    *
    * @tparam Node The AST node with the capturing groups
    */
    template<typename Node>
    struct capture_name_spec
    {
        using type = type_sequence<>;
    };

    template<typename Node>
    using capture_name_spec_t = typename capture_name_spec<Node>::type;

    template<template<typename...> typename Wrapper, typename... Inner>
    struct capture_name_spec<Wrapper<Inner ...>>
    {
        using type = concat<capture_name_spec_t<Inner> ...>;
    };

    template<template<match_mode, typename> typename Wrapper, match_mode Mode, typename Inner>
    struct capture_name_spec<Wrapper<Mode, Inner>>
    {
        using type = capture_name_spec_t<Inner>;
    };

    template<match_mode Mode, symbol::quantifier A, symbol::quantifier B, typename Inner>
    struct capture_name_spec<basic_repetition<Mode, A, B, Inner>>
    {
        using type = capture_name_spec_t<Inner>;
    };

    template<std::size_t ID, typename Name, typename Inner>
    struct capture_name_spec<capture<ID, Name, Inner>>
    {
        using type = push<capture_name_spec_t<Inner>, Name>;
    };
}
#endif //MREGEX_AST_TRAITS_HPP